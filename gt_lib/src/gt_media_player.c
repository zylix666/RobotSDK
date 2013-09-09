/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_media_player.h"

#include <time.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>
#include <assert.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>

#include <libavutil/avstring.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>

#include <libavformat/avformat.h>

#include <libswscale/swscale.h>

#include <libavcodec/avcodec.h>
//#include <libavcodec/opt.h>
#include <libavcodec/avfft.h>

#if defined(GT_CONFIG_MEDIA_PLAYER_FFMPEG_PACKET_LOST_COUNTER)
int Ffmpeg_GetPacketLostCount(void);
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
    
AVFormatContext *pFormatCtx = NULL;
int videoStream = -1;
int audioStream = -1;
AVCodecContext *pCodecVideoCtx = NULL;
AVCodec *pCodecVideo = NULL;

AVCodecContext *pCodecAudioCtx = NULL;
AVCodec *pCodecAudio = NULL;

double videoFps = 0;
double videoFrameDelay = 0;

int toStop = 0;
pthread_t threadPlay;
int playStatus = GT_MEDIA_PLAYER_STOP;
gt_utf8 *strPlayUrl = NULL;

pthread_t threadPlayBuffer;
GtBuffer_t *playBuffer = NULL;
    
GtCallbackData_t callbackDataVideo = NULL;
GtCallbackData_t callbackDataAudio = NULL;

AVPacket packet;

gt_byte *audioBuffer = NULL;
int audioBufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE * 2 * 5;
int audioBufferLength = 0;
int audioBufferGot = 0;
GtLock_t *audioBufferLock;
//sem_t audioBufferMutex;
int audioSampleRate = 0;
int audioChannelNumber = 0;
int audioFrameSize = 0;
enum AVSampleFormat audioSampleFormat;
int audioIsLocking = 0;

struct SwsContext *img_convert_ctx = NULL;

AVFrame *pFrame = NULL;
AVFrame *pFrameRGB = NULL;
AVFrame *pFrameAudio = NULL;

int videoWidth = 0;
int videoHeight = 0;
gt_byte *videoBuffer = NULL;
int videoBufferSize = 0;
int videoBufferGot = 0;
GtLock_t *videoBufferLock;
//sem_t videoBufferMutex;

int isVideoFound = 0;
int isAudioFound = 0;

int playOption = GT_MEDIA_PLAYER_PLAY_DEFAULT;

#if defined(GT_CONFIG_ANDROID)
int videoFormat = PIX_FMT_RGB565;
//int videoFormat = PIX_FMT_BGR32;
#else
int videoFormat = PIX_FMT_RGB24;
#endif
    
static void ffmpeg_log_callback(void* ptr, int level, const char* fmt, va_list vl) {
#if defined(GT_CONFIG_ANDROID)
	char str[4096];
	vsprintf(str, fmt, vl);

	__android_log_print(ANDROID_LOG_INFO, "ffmpeg", str);
#else
    vfprintf(stdout, fmt, vl);
#endif
	//GtLog_Log(fmt, vl);
	//GT_LOGI("log_callback");
	//vfprintf(stdout, fmt, vl);
}

int GtMediaPlayer_InitLib(gt_utf8 *license, gt_utf8 *token) {
//#if defined(GT_CONFIG_DEBUG)
//	av_log_set_callback(ffmpeg_log_callback);
//	av_log_set_level(50);
//#else
    av_log_set_level(0);
//#endif

	avcodec_register_all();
	av_register_all();
	avformat_network_init();
    
	GT_LOGI("init FFMPEG");

	audioBuffer = (gt_byte *) malloc(audioBufferSize);
	if (audioBuffer == NULL) {
		return -1;
	}

	pFrame = avcodec_alloc_frame();
	if (pFrame == NULL) {
		return -2;
	}

	pFrameAudio = avcodec_alloc_frame();
	if (pFrameAudio == NULL) {
		return -2;
	}

    GtLock_New(&videoBufferLock);
    GtLock_New(&audioBufferLock);
//	sem_init(&videoBufferMutex, 0, 1);
//	sem_init(&audioBufferMutex, 0, 1);
    
    strPlayUrl = (gt_utf8 *) calloc(1, 1024);

	return 1;
}

/*
int GtMediaPlayer_New(GtMediaPlayer_t **mp) {
	*mp = calloc(1, sizeof(GtMediaPlayer_t));

	if (strPlayUrl == NULL) {
		strPlayUrl = (gt_utf8 *) calloc(1, 1024);
	}

	return 1;
}
 */

int GtMediaPlayer_GetVideoBuffer(int idPlayer, gt_byte *buf, int bufSize) {
	int readLength = 0;

	if (videoBufferSize != bufSize) {
		GT_LOGE("videoBufferSize(%d) != bufSize(%d)", videoBufferSize, bufSize);
		return -1;
	}

    GtLock_Lock(videoBufferLock);
	//sem_wait(&videoBufferMutex);
    
	if (videoBufferGot == 0) {
        //GT_LOGI("videoBufferSize = %d", videoBufferSize);
        memcpy(buf, videoBuffer, videoBufferSize);
        readLength = videoBufferSize;
        videoBufferGot = 1;
	}
	
    GtLock_Unlock(videoBufferLock);
	//sem_post(&videoBufferMutex);

	return readLength;
}

int GtMediaPlayer_GetAudioBuffer(int idPlayer, gt_byte *buf, int bufSize) {
	int readLength = 0;

	if (audioBufferSize != bufSize) {
		GT_LOGE("audioBufferSize(%d) != bufSize(%d)", audioBufferSize, bufSize);
		return -1;
	}

    GtLock_Lock(audioBufferLock);
	//sem_wait(&audioBufferMutex);
    if (audioIsLocking == 1) {
        GT_LOGE("audioIsLocking == 1")
    }
    audioIsLocking = 1;
    
	if (audioBufferGot == 0) {
        memcpy(buf, audioBuffer, audioBufferLength);
        readLength = audioBufferLength;
        audioBufferGot = 1;
	}

    audioIsLocking = 0;
    GtLock_Unlock(audioBufferLock);
	//sem_post(&audioBufferMutex);

	return readLength;
}

int GtMediaPlayer_GetVideoBufferSize(int idPlayer) {
	return videoBufferSize;
}
    
int GtMediaPlayer_GetVideoWidth(int idPlayer) {
    return videoWidth;
}
    
int GtMediaPlayer_GetVideoHeight(int idPlayer) {
    return videoHeight;
}

int GtMediaPlayer_GetAudioBufferSize(int idPlayer) {
	return audioBufferSize;
}

int GtMediaPlayer_GetAudioFrameSize(int idPlayer) {
    return audioFrameSize;
}

int GtMediaPlayer_GetAudioSampleRate(int idPlayer) {
	return audioSampleRate;
}

int GtMediaPlayer_GetAudioChannelNumber(int idPlayer) {
	return audioChannelNumber;
}

int GtMediaPlayer_GetPlayStatus(int idPlayer) {
	return playStatus;
}

int gtMediaPlayer_SetVideoSize(int idPlayer, int width, int height) {
	GT_LOGI("setVideoSize");

	videoWidth = width;
	videoHeight = height;

	GT_LOGI("videoWidth = %d", videoWidth);
    GT_LOGI("videoHeight = %d", videoHeight);

	// Determine required buffer size and allocate buffer
	videoBufferSize = avpicture_get_size(videoFormat, videoWidth, videoHeight);
	GT_LOGI("videoBufferSize = %d", videoBufferSize);

	if (videoBuffer != NULL) {
		free(videoBuffer);
	}

	videoBuffer = (gt_byte *) calloc(1, videoBufferSize);
	if (videoBuffer == NULL) {
        GT_LOGE("videoBuffer == NULL");
		return -2;
	}

	if (pFrameRGB != NULL) {
		av_free(pFrameRGB);
	}

	pFrameRGB = avcodec_alloc_frame();
	if (pFrameRGB == NULL) {
        GT_LOGE("pFrameRGB == NULL");
		return -3;
	}

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	avpicture_fill((AVPicture *) pFrameRGB, videoBuffer, videoFormat, videoWidth, videoHeight);
	//avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, videoWidth, videoHeight);

	return 1;
}

static int interrupt_cb(void *ctx) {
    //GT_LOGI("interrupt_cb");
    
    if (toStop == 0) {
        return 0;
    }
    
    return 1;
}
    
static const AVIOInterruptCB int_cb = { interrupt_cb, NULL }; 

void *threadRoutinePlay(void *ptr) {
	GT_LOGI("threadRoutinePlay = %s", strPlayUrl);
    
    int idPlayer = *((int *)ptr);

	int i;
int audioDecodedSize;
#if defined(GT_CONFIG_MEDIA_PLAYER_FFMPEG_PACKET_LOST_COUNTER)
    int packetLostCountNew = Ffmpeg_GetPacketLostCount();
    int packetLostCount = packetLostCountNew;
    int keyFrameCount = 0;
#else
    int packetLostCountNew = 0;
    int packetLostCount = 0;
    int keyFrameCount = 0;
#endif

	pFormatCtx = avformat_alloc_context();
	if (pFormatCtx == NULL) {
		GT_LOGE("pFormatCtx == NULL");
		playStatus = GT_MEDIA_PLAYER_ERROR_ALLOC_FORMAT;
        goto stopThreadRoutingPlay;
	}
    pFormatCtx->interrupt_callback = int_cb; 

	// Open video file
	// rtsp://www.rcc.ryerson.ca:554/bfortner/tofcpug/jan2007/file1.mov
	// rtsp://quicktime.uvm.edu:1554/waw/wdi05hs2b.mov

    GT_LOGI("playOption=%d", playOption);
    
	AVDictionary *opts = NULL;

	if (playOption == GT_MEDIA_PLAYER_PLAY_UDP) {
		av_dict_set(&opts, "max_delay", "500000", 0);  // max reordering delay of 0.5 seconds
		av_dict_set(&opts, "rtsp_transport", "udp", 0);
		//av_dict_set(&opts, "timeout", "20", 0);
	} else if (playOption == GT_MEDIA_PLAYER_PLAY_TCP) {
		av_dict_set(&opts, "rtsp_transport", "tcp", 0);
		//av_dict_set(&opts, "timeout", "20", 0);
	} else if (playOption == GT_MEDIA_PLAYER_PLAY_HTTP) {
		av_dict_set(&opts, "rtsp_transport", "http", 0);
		//av_dict_set(&opts, "timeout", "20", 0);
	} else {
#if defined(GT_CONFIG_MEDIA_PLAYER_FFMPEG_DEFAULT_TCP)
        av_dict_set(&opts, "rtsp_transport", "tcp", 0);
#endif
	}

	int ret = avformat_open_input(&pFormatCtx, (const char *)strPlayUrl, NULL, &opts);
	//int ret = avformat_open_input(&pFormatCtx, "rtsp://quicktime.uvm.edu:1554/waw/wdi05hs2b.mov", NULL, NULL);
	//int ret = avformat_open_input(&pFormatCtx, "http://www.leaderg.com/temp/h264.ts", NULL, NULL);
	//int ret = avformat_open_input(&pFormatCtx, "http://www.leaderg.com/temp/sophie.mov", NULL, NULL);
	//int ret = avformat_open_input(&pFormatCtx, "rtsp://192.168.0.3/cam1/h264?user=root&pwd=root", NULL, NULL);
	//int ret = avformat_open_input(&pFormatCtx, "rtsp://192.168.0.3/cam1/h264?user=root&pwd=root", NULL, NULL);
	//int ret = av_open_input_file(&pFormatCtx, "rtsp://192.168.0.3/cam1/h264?user=root&pwd=root", NULL, 4096, NULL);
	//int ret = av_open_input_file(&pFormatCtx, "rtsp://192.168.0.3/cam1/h264?user=root&pwd=root", NULL, 4096, NULL);
	//int ret = av_open_input_file(&pFormatCtx, "rtsp://quicktime.uvm.edu:1554/waw/wdi05hs2b.mov", NULL, 4096, NULL);
	//int ret = avformat_open_input(&pFormatCtx, "rtsp://192.168.0.2/cam1/mpeg4?user=root&pwd=root&ext=mpeg4", NULL, NULL);

	av_dict_free(&opts);

	if (ret != 0) {
		GT_LOGE("avformat_open_input = %d", ret);
        char strError[1024];
        av_strerror(ret, strError, 1024);
        GT_LOGE("avformat_open_input = %s", strError);
        
		playStatus = GT_MEDIA_PLAYER_ERROR_OPEN;
        goto stopThreadRoutingPlay;
	}

	ret = avformat_find_stream_info(pFormatCtx, 0);
	if (ret < 0) {
		GT_LOGE("av_find_stream_info = %d", ret);
		playStatus = GT_MEDIA_PLAYER_ERROR_FIND_STREAM_INFO;
        goto stopThreadRoutingPlay;
	}

	//av_dump_format(pFormatCtx, 0, pFormatCtx->filename, 0);

	// Find the first video stream
	videoStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
            isVideoFound = 1;
			break;
		}
	}

	if (videoStream == -1) {
		GT_LOGE("Didn't find a video stream");
	}

	if (isVideoFound) {
		// Get a pointer to the codec context for the video stream
		pCodecVideoCtx = pFormatCtx->streams[videoStream]->codec;

		// Find the decoder for the video stream
		pCodecVideo = avcodec_find_decoder(pCodecVideoCtx->codec_id);
		if (pCodecVideo == NULL) {
			GT_LOGE("Video codec not found, codec_id=0x%x", pCodecVideoCtx->codec_id);
			playStatus = GT_MEDIA_PLAYER_ERROR_FIND_VIDEO_DECODER;
            goto stopThreadRoutingPlay;
		}
	}

	if (isVideoFound) {
		GT_LOGI(pCodecVideo->name);
        GT_LOGI("GtRuntime_GetCpuCount()=%d", GtRuntime_GetCpuCount());
		//pCodecVideoCtx->thread_count = GtCpu_GetCount();
		//pCodecVideoCtx->thread_type = FF_THREAD_FRAME;

		/*
		 int frame_threading_supported = (pCodecVideoCtx->codec->capabilities & CODEC_CAP_FRAME_THREADS) && !(pCodecVideoCtx->flags & CODEC_FLAG_TRUNCATED)
		 && !(pCodecVideoCtx->flags & CODEC_FLAG_LOW_DELAY) && !(pCodecVideoCtx->flags2 & CODEC_FLAG2_CHUNKS);

		 if (pCodecVideoCtx->thread_count <= 1) {
		 GT_LOGI("active_thread_type = 0");
		 //pCodecVideoCtx->active_thread_type = 0;
		 } else if (frame_threading_supported && (pCodecVideoCtx->thread_type & FF_THREAD_FRAME)) {
		 GT_LOGI("active_thread_type = FF_THREAD_FRAME");
		 //pCodecVideoCtx->active_thread_type = FF_THREAD_FRAME;
		 } else {
		 GT_LOGI("active_thread_type = FF_THREAD_SLICE");
		 //pCodecVideoCtx->active_thread_type = FF_THREAD_SLICE;
		 }
		 */

// Inform the codec that we can handle truncated bitstreams -- i.e.,
// bitstreams where frame boundaries can fall in the middle of packets
//if (pCodecVideo->capabilities & CODEC_CAP_TRUNCATED) {
//	pCodecVideoCtx->flags |= CODEC_FLAG_TRUNCATED;
//}
//1. 必须显式设置CODEC_CAP_FRAME_THREADS；
//2. 必须屏蔽CODEC_FLAG_TRUNCATED；
//3. 必须屏蔽CODEC_FLAG_LOW_DELAY；
//4. 必须屏蔽CODEC_FLAG2_CHUNKS。
// Open video codec
		if (avcodec_open2(pCodecVideoCtx, pCodecVideo, 0) < 0) {
			GT_LOGE("Could not open video codec, codec_id=0x%x", pCodecVideoCtx->codec_id);
			playStatus = GT_MEDIA_PLAYER_ERROR_OPEN_VIDEO_DECODER;
            goto stopThreadRoutingPlay;
		}
	}
    
    if (pCodecVideoCtx->width <= 0 || pCodecVideoCtx->height <= 0) {
		playStatus = GT_MEDIA_PLAYER_ERROR_NO_VIDEO;
        goto stopThreadRoutingPlay;
	}

	if (isVideoFound) {
		GT_LOGI("pCodecVideoCtx->width=%d, pCodecVideoCtx->height=%d", pCodecVideoCtx->width, pCodecVideoCtx->height);
        gtMediaPlayer_SetVideoSize(idPlayer, pCodecVideoCtx->width, pCodecVideoCtx->height);

		videoFps = av_q2d(pFormatCtx->streams[videoStream]->r_frame_rate);
		GT_LOGI("videoFps=%lf", videoFps);

		videoFrameDelay = av_q2d(pCodecVideoCtx->time_base);
		GT_LOGI("videoFrameDelay=%lf", videoFrameDelay);

		// av_seek_frame(pFormatCtx, opt.streamId, opt.lstart, AVSEEK_FLAG_ANY);
	}

	// Find the first audio stream
	audioStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStream = i;
            isAudioFound = 1;
			break;
		}
	}

	if (audioStream == -1) {
		GT_LOGE("Didn't find a audio stream");
		isAudioFound = 0;
	}

	if (isAudioFound) {
		// Get a pointer to the codec context for the audio stream
		pCodecAudioCtx = pFormatCtx->streams[audioStream]->codec;

		GT_LOGI("pCodecAudioCtx->codec_id = 0x%x", pCodecAudioCtx->codec_id);
		// AV_CODEC_ID_MP2 = 0x15000, AV_CODEC_ID_MP3, AV_CODEC_ID_AAC, AV_CODEC_ID_AC3,

		// Find the decoder for the audio stream
		pCodecAudio = avcodec_find_decoder(pCodecAudioCtx->codec_id);
		if (pCodecAudio == NULL) {
			GT_LOGE("pCodecAudio == NULL, codec_id=0x%x", pCodecAudioCtx->codec_id);
			playStatus = GT_MEDIA_PLAYER_ERROR_FIND_AUDIO_DECODER;
            goto stopThreadRoutingPlay;
		}
	}

	if (isAudioFound) {
		GT_LOGI(pCodecAudio->name);

		// Inform the codec that we can handle truncated bitstreams -- i.e.,
		// bitstreams where frame boundaries can fall in the middle of packets
		if (pCodecAudio->capabilities & CODEC_CAP_TRUNCATED) {
			pCodecAudioCtx->flags |= CODEC_FLAG_TRUNCATED;
		}

		// Open codec
		if (avcodec_open2(pCodecAudioCtx, pCodecAudio, 0) < 0) {
			playStatus = GT_MEDIA_PLAYER_ERROR_OPEN_AUDIO_DECODER;
            goto stopThreadRoutingPlay;
		}
	}

	if (isAudioFound) {
		audioSampleRate = pCodecAudioCtx->sample_rate;
		audioChannelNumber = pCodecAudioCtx->channels;
        audioFrameSize = pCodecAudioCtx->frame_size;
        audioSampleFormat = pCodecAudioCtx->sample_fmt;
		GT_LOGI("sample_rate = %d", pCodecAudioCtx->sample_rate);
        GT_LOGI("channels = %d", pCodecAudioCtx->channels);
        GT_LOGI("frame_size = %d", pCodecAudioCtx->frame_size);
        GT_LOGI("audioSampleFormat = %d", pCodecAudioCtx->sample_fmt);
	}

	if (isVideoFound) {
		img_convert_ctx = sws_getContext(pCodecVideoCtx->width, pCodecVideoCtx->height, pCodecVideoCtx->pix_fmt, videoWidth, videoHeight, videoFormat,
				SWS_FAST_BILINEAR, NULL, NULL, NULL);
		if (img_convert_ctx == NULL) {
			playStatus = GT_MEDIA_PLAYER_ERROR_GET_VIDEO_CONVERTER;
            goto stopThreadRoutingPlay;
		}
	}

	if (isVideoFound == 1 && isAudioFound == 1) {
		playStatus = GT_MEDIA_PLAYER_PLAY_VIDEO_AUDIO;
	} else if (isVideoFound == 1) {
		playStatus = GT_MEDIA_PLAYER_PLAY_VIDEO;
	} else if (isAudioFound == 1) {
		playStatus = GT_MEDIA_PLAYER_PLAY_AUDIO;
	} else {
		playStatus = GT_MEDIA_PLAYER_ERROR_NO_VIDEO_AUDIO;
        goto stopThreadRoutingPlay;
	}

	//av_read_play(pFormatCtx);  // This will cause dummy viewer in Uniform IP CAM

	int gotVideoFrame = 0;
	int gotAudioFrame = 0;
	while (toStop == 0) {
		ret = av_read_frame(pFormatCtx, &packet);
        ret = ret;

        if (packet.size <= 0) {
            GT_LOGI("packet.size <= 0 (%d)", packet.size);
        }
		else if (packet.stream_index == audioStream && isAudioFound == 1) {
            avcodec_get_frame_defaults(pFrameAudio);
			avcodec_decode_audio4(pCodecAudioCtx, pFrameAudio, &gotAudioFrame, &packet);

			if (gotAudioFrame) {
                audioDecodedSize = av_samples_get_buffer_size(NULL, audioChannelNumber,
                                                              pFrameAudio->nb_samples,
                                                              pCodecAudioCtx->sample_fmt, 1);
				//GT_LOGI("audioDecodedSize=%d", audioDecodedSize);
                
                GtLock_Lock(audioBufferLock);
                if (audioIsLocking == 1) {
                    GT_LOGE("audioIsLocking == 1")
                }
                audioIsLocking = 1;

				if (audioDecodedSize <= audioBufferSize) {                    
					if (audioBufferGot == 1) {
						audioBufferLength = audioDecodedSize;
						memcpy(audioBuffer, pFrameAudio->data[0], audioBufferLength);
						audioBufferGot = 0;
					} else {
						if ((audioBufferLength + audioDecodedSize) <= audioBufferSize) {
							//GT_LOGI("Append audio buffer previous len = %d", audioBufferLength);
							memcpy(audioBuffer + audioBufferLength, pFrameAudio->data[0], audioDecodedSize);
							audioBufferLength += audioDecodedSize;
						} else {
							GT_LOGE(
									"(audioBufferLength + audioDecodedSize)(%d) > audioBufferSize(%d)", (audioBufferLength + audioDecodedSize), audioBufferSize);
							audioBufferLength = audioDecodedSize;
							memcpy(audioBuffer, pFrameAudio->data[0], audioBufferLength);
						}
					}

					if (callbackDataAudio != NULL) {
						callbackDataAudio(pFrameAudio->data[0], audioDecodedSize);
					}
				} else {
					GT_LOGE("pFrameAudio->audioDecodedSize(%d) > audioBufferSize(%d)", audioDecodedSize, audioBufferSize);
				}
                audioIsLocking = 0;
                GtLock_Unlock(audioBufferLock);
				//sem_post(&audioBufferMutex);
			}
		} else if (packet.stream_index == videoStream && isVideoFound == 1) {
			avcodec_decode_video2(pCodecVideoCtx, pFrame, &gotVideoFrame, &packet);
			if (gotVideoFrame) {
#if defined(GT_CONFIG_MEDIA_PLAYER_FFMPEG_PACKET_LOST_COUNTER)
                packetLostCountNew = Ffmpeg_GetPacketLostCount();
#endif
                if (packetLostCount != packetLostCountNew) {
                    packetLostCount = packetLostCountNew;
                    keyFrameCount = 2;
                    GT_LOGI("packetLostCount=%d", packetLostCount);
                }
                
                if (keyFrameCount > 0 && pFrame->key_frame == 0) {
                    GT_LOGI("drop non key frame");
                    goto continueThreadRoutinePlay;
                } else if (keyFrameCount > 0){
                    GT_LOGI("key frame found");
                        
                    keyFrameCount --;
                        
                    if (keyFrameCount >= 1) {
                        GT_LOGI("drop key frame");
                        goto continueThreadRoutinePlay;
                    }
                }
                
				//GT_LOGI("gotVideoFrame");
                GtLock_Lock(videoBufferLock);
				//sem_wait(&videoBufferMutex);
                if (videoBufferGot == 1) {
                    //GT_LOGI("gotVideoFrame 1");
                    sws_scale(img_convert_ctx, (uint8_t const * const *) pFrame->data, pFrame->linesize, 0, pCodecVideoCtx->height, pFrameRGB->data,
						pFrameRGB->linesize);
                    //GT_LOGI("gotVideoFrame 2");
                    videoBufferGot = 0;
                    if (callbackDataVideo != NULL) {
                        callbackDataVideo(videoBuffer, videoBufferSize);
                    }
                    //GT_LOGI("gotVideoFrame 3");
                } else {
                    //GT_LOGI("skip frame");
                }
                GtLock_Unlock(videoBufferLock);
				//sem_post(&videoBufferMutex);
			}
		}

continueThreadRoutinePlay:
		av_free_packet(&packet);
	}

	//av_read_pause(pFormatCtx);
    
    playStatus = GT_MEDIA_PLAYER_STOP;
    
	GT_LOGI("playStatus = GT_MEDIA_PLAYER_STOP");

stopThreadRoutingPlay:

    toStop = 1;

	av_free_packet(&packet);

	if (img_convert_ctx != NULL) {
		sws_freeContext(img_convert_ctx);
		img_convert_ctx = NULL;
	}

	if (pCodecVideoCtx != NULL) {
		avcodec_close(pCodecVideoCtx);
		pCodecVideoCtx = NULL;
	}

	if (pCodecAudioCtx != NULL) {
		avcodec_close(pCodecAudioCtx);
		pCodecAudioCtx = NULL;
	}

	if (pFormatCtx != NULL) {
		avformat_close_input(&pFormatCtx);
		pFormatCtx = NULL;
	}
       
    GtLock_Lock(videoBufferLock);
    videoBufferGot = 1;
    GtLock_Unlock(videoBufferLock);
       
    GtLock_Lock(audioBufferLock);
    audioBufferGot = 1;
    GtLock_Unlock(audioBufferLock);
    
    return NULL;
}
    
void *threadRoutinePlayAudioBuffer(void *ptr) {
    GT_LOGI("threadRoutinePlayBuffer");
    
    int len;
    uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
        
        av_init_packet(&packet);
        
        /* find the mpeg audio decoder */
        pCodecAudio = avcodec_find_decoder(AV_CODEC_ID_MP3);
        if (!pCodecAudio) {
            fprintf(stderr, "Codec not found\n");
            goto stopThreadRoutingPlayAudioBuffer;
        }
        
        pCodecAudioCtx = avcodec_alloc_context3(pCodecAudio);
        
        /* open it */
        if (avcodec_open2(pCodecAudioCtx, pCodecAudio, NULL) < 0) {
            GT_LOGE("Could not open codec");
            goto stopThreadRoutingPlayAudioBuffer;
        }
        
        GtBuffer_Lock(playBuffer);
        packet.size = GtBuffer_Get(playBuffer, inbuf, AUDIO_INBUF_SIZE);
        packet.data = inbuf;
        GtBuffer_Unlock(playBuffer);
    
        int gotAudioFrame;
    int audioDecodedSize;
    
        while (packet.size > 0) {
            if (toStop == 1) {
                break;
            }
            
            gotAudioFrame = 0;
            
            if (!pFrameAudio) {
                if (!(pFrameAudio = avcodec_alloc_frame())) {
                    GT_LOGE("Could not allocate audio frame");
                    playStatus = GT_MEDIA_PLAYER_ERROR;
                    goto stopThreadRoutingPlayAudioBuffer;
                }
            } else {
                avcodec_get_frame_defaults(pFrameAudio);
            }
            
            len = avcodec_decode_audio4(pCodecAudioCtx, pFrameAudio, &gotAudioFrame, &packet);
            if (len < 0) {
                GT_LOGE("Error while decoding");
                usleep(1000000);
                continue;
            }
            if (gotAudioFrame) {
                /* if a frame has been decoded, output it */
                audioDecodedSize = av_samples_get_buffer_size(NULL,
                                                              pCodecAudioCtx->channels,
                                                              pFrameAudio->nb_samples,
                                                              pCodecAudioCtx->sample_fmt,
                                                              1);
                GT_LOGI("audioDecodedSize = %d", audioDecodedSize);
                
                if (audioDecodedSize <= audioBufferSize) {
                    if (audioBufferGot == 1) {
                        audioBufferLength = audioDecodedSize;
                        memcpy(audioBuffer, pFrameAudio->data[0], audioBufferLength);
                        audioBufferGot = 0;
                    } else {
                        if ((audioBufferLength + audioDecodedSize) <= audioBufferSize) {
                            //GT_LOGI("Append audio buffer previous len = %d", audioBufferLength);
                            memcpy(audioBuffer + audioBufferLength, pFrameAudio->data[0], audioDecodedSize);
                            audioBufferLength += audioDecodedSize;
                        } else {
                            GT_LOGE(
                                    "(audioBufferLength + audioDecodedSize)(%d) > audioBufferSize(%d)", (audioBufferLength + audioDecodedSize), audioBufferSize);
                            audioBufferLength = audioDecodedSize;
                            memcpy(audioBuffer, pFrameAudio->data[0], audioBufferLength);
                        }
                    }
                    
                    if (callbackDataAudio != NULL) {
                        callbackDataAudio(pFrameAudio->data[0], audioDecodedSize);
                    }
                } else {
                    GT_LOGE("pFrameAudio->audioDecodedSize(%d) > audioBufferSize(%d)", audioDecodedSize, audioBufferSize);
                }
                audioIsLocking = 0;
                GtLock_Unlock(audioBufferLock);
                              
                //fwrite(decoded_frame->data[0], 1, data_size, outfile);
            }
            packet.size -= len;
            packet.data += len;
            packet.dts = AV_NOPTS_VALUE;
            packet.pts = AV_NOPTS_VALUE;
            if (packet.size < AUDIO_REFILL_THRESH) {
                /* Refill the input buffer, to avoid trying to decode
                 * incomplete frames. Instead of this, one could also use
                 * a parser, or use a proper container format through
                 * libavformat. */
                memmove(inbuf, packet.data, packet.size);
                packet.data = inbuf;
                GtBuffer_Lock(playBuffer);
                len = GtBuffer_Get(playBuffer, inbuf, AUDIO_INBUF_SIZE - packet.size);
                GtBuffer_Unlock(playBuffer);
                if (len > 0) {
                    packet.size += len;
                }
            }
        }
    
        playStatus = GT_MEDIA_PLAYER_STOP;
        GT_LOGI("playStatus = GT_MEDIA_PLAYER_STOP");
    
stopThreadRoutingPlayAudioBuffer:
    toStop = 1;
    
        av_free_packet(&packet);
    
        avcodec_close(pCodecAudioCtx);
        av_free(pCodecAudioCtx);
        avcodec_free_frame(&pFrameAudio);
    
        return NULL;
}

int GtMediaPlayer_PlayAudioBuffer(int idPlayer, GtBuffer_t *buffer, int option) {
    if (playStatus >= 1) {
        GT_LOGE("isPlaying");
        return GT_MEDIA_PLAYER_ERROR_IS_PLAYING;
    }
    if (buffer == NULL) {
        GT_LOGE("buffer == NULL");
        return GT_MEDIA_PLAYER_ERROR_NULL_POINTER;
    }
    playBuffer = buffer;
    playStatus = GT_MEDIA_PLAYER_PREPARE_PLAY;
    toStop = 0;
    videoBufferGot = 1;
    audioBufferGot = 1;
        
    playOption = option;
    GT_LOGI("playOption=%d", playOption);
        
    //strcpy((char *)strPlayUrl, (const char *)strUrl); //IT SHOULD BE GtUtf8_Copy;
        
    if (pFormatCtx != NULL) {
        avformat_close_input(&pFormatCtx);
        pFormatCtx = NULL;
    }
    videoStream = -1;
    audioStream = -1;
    if (pCodecVideoCtx != NULL) {
        avcodec_close(pCodecVideoCtx);
        pCodecVideoCtx = NULL;
    }
    pCodecVideo = NULL;
        
    if (pCodecAudioCtx != NULL) {
        avcodec_close(pCodecAudioCtx);
        pCodecAudioCtx = NULL;
    }
    pCodecAudio = NULL;
        
    videoFps = 0;
    videoFrameDelay = 0;
        
    GtLock_Lock(audioBufferLock);
    //sem_wait(&audioBufferMutex);
    if (audioIsLocking == 1) {
        GT_LOGE("audioIsLocking == 1")
    }
    audioIsLocking = 1;
        
    audioBufferGot = 1;
        
    audioIsLocking = 0;
    GtLock_Unlock(audioBufferLock);
    //sem_post(&audioBufferMutex);
        
    audioSampleRate = 0;
    audioChannelNumber = 0;
        
    if (img_convert_ctx != NULL) {
        sws_freeContext(img_convert_ctx);
        img_convert_ctx = NULL;
    }
        
    GtLock_Lock(videoBufferLock);
    //sem_wait(&videoBufferMutex);
    videoBufferGot = 1;
    GtLock_Unlock(videoBufferLock);
    //sem_post(&videoBufferMutex);
        
    av_free_packet(&packet);
        
    struct sched_param   param;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
        
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  // Schedule in system scope.
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
        
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = (sched_get_priority_max(SCHED_RR) * 3 + sched_get_priority_min(SCHED_RR))/4;
    pthread_attr_setschedparam (&attr, &param);
        
    int ret = pthread_create(&threadPlayBuffer, &attr, threadRoutinePlayAudioBuffer, &idPlayer);
    if (ret) {
        GT_LOGE("pthread_create=%d", ret);
        playStatus = GT_MEDIA_PLAYER_ERROR_THREAD;
        return GT_MEDIA_PLAYER_ERROR_THREAD;
    }
        
    pthread_attr_destroy(&attr);
        
    GT_LOGI("GtMediaPlayer_PlayBuffer");
    return GT_MEDIA_PLAYER_PREPARE_PLAY;
}
   
int GtMediaPlayer_Play(int idPlayer, gt_utf8 *strUrl, int option) {
	if (playStatus >= 1) {
		GT_LOGE("isPlaying");
		return GT_MEDIA_PLAYER_ERROR_IS_PLAYING;
	}
	playStatus = GT_MEDIA_PLAYER_PREPARE_PLAY;
	toStop = 0;
    videoBufferGot = 1;
    audioBufferGot = 1;

    playOption = option;
    GT_LOGI("playOption=%d", playOption);

	strcpy((char *)strPlayUrl, (const char *)strUrl); //IT SHOULD BE GtUtf8_Copy;

	if (pFormatCtx != NULL) {
		avformat_close_input(&pFormatCtx);
		pFormatCtx = NULL;
	}
	videoStream = -1;
	audioStream = -1;
	if (pCodecVideoCtx != NULL) {
		avcodec_close(pCodecVideoCtx);
		pCodecVideoCtx = NULL;
	}
	pCodecVideo = NULL;

	if (pCodecAudioCtx != NULL) {
		avcodec_close(pCodecAudioCtx);
		pCodecAudioCtx = NULL;
	}
	pCodecAudio = NULL;

	videoFps = 0;
	videoFrameDelay = 0;

    GtLock_Lock(audioBufferLock);
	//sem_wait(&audioBufferMutex);
    if (audioIsLocking == 1) {
        GT_LOGE("audioIsLocking == 1")
    }
    audioIsLocking = 1;
    
	audioBufferGot = 1;
    
    audioIsLocking = 0;
    GtLock_Unlock(audioBufferLock);
	//sem_post(&audioBufferMutex);
    
	audioSampleRate = 0;
	audioChannelNumber = 0;

	if (img_convert_ctx != NULL) {
		sws_freeContext(img_convert_ctx);
		img_convert_ctx = NULL;
	}

    GtLock_Lock(videoBufferLock);
	//sem_wait(&videoBufferMutex);
	videoBufferGot = 1;
    GtLock_Unlock(videoBufferLock);
	//sem_post(&videoBufferMutex);

	av_free_packet(&packet);
    
    struct sched_param   param;
    pthread_attr_t attr;    
    pthread_attr_init(&attr);
    
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  // Schedule in system scope.
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = (sched_get_priority_max(SCHED_RR) * 3 + sched_get_priority_min(SCHED_RR))/4;
    pthread_attr_setschedparam (&attr, &param);

	int ret = pthread_create(&threadPlay, &attr, threadRoutinePlay, &idPlayer);
	if (ret) {
		GT_LOGE("pthread_create=%d", ret);
		playStatus = GT_MEDIA_PLAYER_ERROR_THREAD;
		return GT_MEDIA_PLAYER_ERROR_THREAD;
	}
    
    pthread_attr_destroy(&attr);

	GT_LOGI("GtMediaPlayer_Play");
	return GT_MEDIA_PLAYER_PREPARE_PLAY;
}
    
int GtMediaPlayer_Stop(int idPlayer) {
	toStop = 1;
    
    //if (playStatus < 0) {
        //playStatus = GT_MEDIA_PLAYER_STOP;
    //}
	GT_LOGI("GtMediaPlayer_Stop");
	return 1;
}

/*
int GtMediaPlayer_Free(int idPlayer) {
	if (mp == NULL)
		return 1;

	free(mp);

	GT_LOGI("GtMediaPlayer_Free");
	return 1;
}
*/

int GtMediaPlayer_SetVideoCallback(int idPlayer, GtCallbackData_t callbackData) {
	callbackDataVideo = callbackData;

	return 1;
}

int GtMediaPlayer_SetAudioCallback(int idPlayer, GtCallbackData_t callbackData) {
	callbackDataAudio = callbackData;

	return 1;
}

#ifdef __cplusplus
}
#endif
