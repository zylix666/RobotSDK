/*
   Copyright (c) LEADERG Co., Ltd. All rights reserved.
   Free of charge for noncommercial purposes.
   Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
   THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_sound.h"
#define WAV_HEADER_SIZE 46
#define CHANNEL_OFFSET 22
#define SAMPLE_RATE_OFFSET 24
#define BITS_PER_SAMPLE_OFFSET 34
#define DATA_SIZE_OFFSET 42
#ifdef __cplusplus
extern "C" 
{
#endif
	void voiceRemovedSilence(GtSound_t * sound);

	static int gtSoundIsInit = 0;

	int GtSound_InitLib() {
		if (gtSoundIsInit == 1) {
			return GT_OK_INIT_ALREADY;
		}
		gtSoundIsInit = 1;

		return GT_OK;
	}
	int GtSound_LoadFromFile(GtSound_t * sound , gt_utf8 * path)
	{
		fprintf(stderr,"Start to load wav file, this code currently works for single channel only...\n");
		FILE * f = fopen((char*)path,"rb");
		int i;
		int sz  ;
		short temp ;
		int hz ; 
		fseek(f , CHANNEL_OFFSET,SEEK_SET);
		fread(&temp ,1 ,2  , f);
		sound->channel = temp;
		fseek(f,SAMPLE_RATE_OFFSET , SEEK_SET);
		fread(&hz , 1 , 4 , f);
		sound->hz = hz;
		fseek(f , BITS_PER_SAMPLE_OFFSET , SEEK_SET);
		fread(&temp , 1 , 2 , f);
		sound->bit = temp;
		fseek(f,0,SEEK_END);
		fseek(f,DATA_SIZE_OFFSET,SEEK_SET);
		fread(&sz ,  4 ,1, f);
		sound->dataLength = sz/(sound->bit/(double)8);
		sound->data = (SOUND_T*) malloc(sizeof(SOUND_T) * sound->dataLength);
		fseek(f , WAV_HEADER_SIZE , SEEK_SET);
		for(i = 0 ; i < sound->dataLength ; i++){
			if (sound->bit == 8)
				fread(&temp , 1 , 1 , f);
			else
				fread(&temp , 1 , 2 , f);
			
			sound->data[i] =(double)temp;
		}
//		voiceRemovedSilence(sound);
		return 1;
	}
	double GtSound_GetMainTone(GtSound_t * sound)
	{
		int i,j;
		int N = 1;
		int dataLength = sound->dataLength;
		SOUND_T * fre;
		for( i = 1; i <= 31 ; i ++){
			N = N * 2;
			if (N == dataLength){

				fre = (SOUND_T*)malloc(sizeof(SOUND_T)*N);
				memcpy(fre , sound->data , sizeof(SOUND_T)*N);
				break;
			}else if( N > dataLength){
				fre = (SOUND_T*)malloc(sizeof(SOUND_T)*N);
				memcpy(fre , sound->data , sizeof(SOUND_T)*dataLength);
				for( j = dataLength ; j < N ; j++ ){
					fre[j] = 0;			
				}
				break;
			}
		}
		if (i > 31){
			fprintf(stderr,"*****Warning too many data, can not fit fft, need to modify code*****\n");
			exit(1);
		}

		SOUND_T * fre_complex = (SOUND_T*) calloc(N,sizeof(SOUND_T));
		FFT(fre , fre_complex , N , 1);
		sound->fft = fre;
		sound->Nfft = N;
		double MaxMag = -1000;
		int MaxIndex = -1;
		for(i =0 ; i< sound->dataLength ; i ++ ){
			double mag = sqrt(fre[i]*fre[i]+fre_complex[i]*fre_complex[i]);
			if (MaxMag < mag){
				MaxMag = mag;
				MaxIndex = i;
			}
		}
		free(fre_complex);
		
		return (MaxIndex)*sound->hz/(double)N;
	}
	void GtSound_PrintInfo(GtSound_t * sound)
	{
		printf("****Sampling Rate       : %d\n",sound->hz);
		printf("****Bits Per Sample     : %d\n",sound->bit);
		printf("****Number of Channels  : %d\n",sound->channel);
		printf("****Data Length		: %d\n" , sound->dataLength);
	
	}
/*
	void voiceRemovedSilence(GtSound_t * sound)
	{	
		int i , count=0;
		int n = sound->dataLength;
		double threshold = 0.3;
		double mean= 0;
		double sd = 0;
		short * voiced = (short * )calloc(n,sizeof(short));
		for(i = 0; i < n ; i ++ ){
			mean+=sound->data[i];
		}
		mean/=n;
		for(i = 0; i < n ; i ++){
			sd+= (sound->data[i]-mean)*(sound->data[i]-mean);
		}
		sd = sqrt(sd/n);
		printf("%lf %lf\n",mean,sd);
		for(i = 0; i < n ; i ++){
			if (abs(sound->data[i]-mean)/sd > threshold){
				voiced[i] = 1;
				count++;
			}
		}
		SOUND_T *new_data = calloc(count , sizeof(SOUND_T));
		int k = 0;
		for(i = 0; i < n ; i ++){
			if( voiced[i] == 1) 
				new_data[k++] = (sound->data[i]-mean)/sd;
		}
		printf("%d\n",count);
		free(sound->data);
		sound->data = new_data;
		sound->dataLength = count;	
	}
*/
	void FFT(double x[] , double y[] , int n , int sign)
	{
		#ifdef DEBUG
			printf("Start to FFT the audio data...number of FFT frames %d\nthis uses zeros-padding FFT for real input which means speed can be improved\n",n);
		#endif
		int i,j,k,l,m,n1,n2;
		double c,c1,e,s,s1,t,tr,ti;
		//Calculate i = log2N
		for(j = 1,i = 1; i<31; i++)
		{
			m = i;
			j = 2*j;
		
			if(j == n)
				break;
		}
		//计算蝶形图的输入下标（码位倒读）
		n1 = n - 1;
		for(j=0,i=0; i<n1; i++)
		{
			if(i<j)           
			{
				tr = x[j];
				ti = y[j];
				x[j] = x[i];
				y[j] = y[i]; 
				x[i] = tr;
				y[i] = ti;                 
			}
			k = n/2;
			while(k<(j+1))
			{
				j = j - k;
				k = k/2;              
			}
			j = j + k;
		}
		//计算每一级的输出，l为某一级，i为同一级的不同群，使用同一内存（即位运算）
		n1 = 1;
		for(l=1; l<=m; l++)
		{
			n1 = 2*n1;
			n2 = n1/2;
			e = 3.1415926535/n2;
			c = 1.0;
			s = 0.0;
			c1 = cos(e);
			s1 = -sign*sin(e);
			for(j=0; j<n2; j++)
			{
				for(i=j; i<n; i+=n1)         
				{
					k = i + n2;
					tr = c*x[k] - s*y[k];
					ti = c*y[k] + s*x[k];
					x[k] = x[i] - tr;
					y[k] = y[i] - ti;
					x[i] = x[i] + tr;
					y[i] = y[i] + ti;        
				}
				t = c;
				c = c*c1 - s*s1;
				s = t*s1 + s*c1;
			}
		}
		//如果是求IFFT，再除以N
		if(sign == -1)
		{
			for(i=0; i<n; i++)
			{
				x[i] /= n;
				y[i] /= n;
			}
		}
	}
	/*
	   GtSound_t *GtSound_New() {

	   if (gtSoundIsInit == 0) return NULL;

	   GtSound_t *sound = (GtSound_t *)gt_calloc(1, sizeof(GtSound_t));
	   if (sound == NULL) {
	   return NULL;
	   }

	   sound->data = gt_calloc(1, sizeof(int));
	   if (sound->data == NULL) {
	   gt_free(sound);
	   return NULL;
	   }

	   sound->magic = GT_MAGIC_SOUND;

	   return sound;
	   }

	   int GtSound_Free(GtSound_t *sound) {
	   if (sound == NULL) return GT_ERROR_NULL_POINTER;
	   if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	   sound->magic = GT_MAGIC_UNKNOWN;

	   gt_free(sound->data);

	   GtLock_Free((GtLock_t *)sound->lock);

	   gt_free(sound);

	   return GT_OK;
	   }

	   int GtSound_Lock(GtSound_t *sound) {
	   if (sound == NULL) return GT_ERROR_NULL_POINTER;
	   if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	   if (sound->lock == NULL) {
	   sound->lock = GtLock_New();
	   if (sound->lock == NULL) {
	   return GT_ERROR_NEW_LOCK;
	   }
	   }

	   return GtLock_Lock((GtLock_t *)sound->lock);
	   }

	   int GtSound_Unlock(GtSound_t *sound) {
	   if (sound == NULL) return GT_ERROR_NULL_POINTER;
	   if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	   return GtLock_Unlock((GtLock_t *)sound->lock);
	   }

	   int GtSound_Test(GtSound_t *sound) {
	   if (sound == NULL) return GT_ERROR_NULL_POINTER;
	   if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	   return GT_OK;
	   }
	 */
#ifdef __cplusplus
}
#endif
