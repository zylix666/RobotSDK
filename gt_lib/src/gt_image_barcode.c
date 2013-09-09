/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_image_barcode.h"

#include "zbar.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtImageBarcodeIsInit = 0;

int GtImageBarcode_InitLib() {
	if (gtImageBarcodeIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtImageBarcodeIsInit = 1;

	zbar_set_verbosity(0);

	return GT_OK;
}

int GtImageBarcode_Decode(GtImage_t *image, gt_utf8 *code, int codeSize) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (image->colorMode != GT_COLOR_MODE_GRAY8) return GT_ERROR_PARAMETER_0;
	if (code == NULL) return GT_ERROR_PARAMETER_1;
	if (codeSize <= 0) return GT_ERROR_PARAMETER_2;
	code[0] = '\0';

	if (gtImageBarcodeIsInit == 0) return GT_ERROR_NOT_INIT;
	
	zbar_image_scanner_t *scanner;
	zbar_image_t *zimage;

	zimage = zbar_image_create();
	if (zimage == NULL) {
		return GT_ERROR_NO_MEMORY;
	}
	zbar_image_set_format(zimage, *(unsigned long *) "Y800");
	zbar_image_set_size(zimage, image->width, image->height);
	zbar_image_set_data(zimage, image->data, image->dataSize, zbar_image_free_data);

	scanner = zbar_image_scanner_create();
	zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);
	int n = zbar_scan_image(scanner, zimage);

	const zbar_symbol_t *symbol = zbar_image_first_symbol(zimage);
	for(; symbol; symbol = zbar_symbol_next(symbol)) {
		zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		const char *data = zbar_symbol_get_data(symbol);
		GT_LOGI("decoded %s symbol \"%s\"\n", zbar_get_symbol_name(typ), data);
		if (strlen(data)< codeSize) {
			sprintf(code, "%s", data);
		}
	}

	zbar_image_destroy(zimage);
	zbar_image_scanner_destroy(scanner);

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
