//
//  ViewController.m
//  gtlib_test_project
//
//  Created by Charles on 9/14/13.
//  Copyright (c) 2013 Charles. All rights reserved.
//

#import "ViewController.h"
#import <gtlib_ios_framework/git_lib_ios.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    [self testGtArray];
    [self testGtColor];
    NSLog(@"ViewController viewDidLoad");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)testGtArray{
    NSLog(@"testGtArray begin------------------------------");
    
    GtArray_InitLib();
    GtArray_t *array = GtArray_New();
    
    gt_utf8 elementType = ' ';
    
    GtArray_Init(array, &elementType, sizeof(int), 5);
    for(int i=0; i<3 ;i++){
        GtArray_Append(array, &i);
    }

    int nLength = GtArray_GetLength(array);
    for(int i=0; i<nLength ;i++){
        void *element = GtArray_Get(array, i);
        NSLog(@"element %d: %d", i, *((int *)element));
    }

    NSLog(@"testGtArray end------------------------------");
}

- (void)testGtColor{
    NSLog(@"testGtColor begin------------------------------");
    GtColor_InitLib();
    
    GtColor_t *color = GtColor_New();
    GtColor_SetRGB888(color, 0xFF, 0xAA, 0x22);
    NSLog(@"color: %d %d %d", color->red8, color->green8, color->blue8);
    
    NSLog(@"testGtColor end------------------------------");
}

@end
