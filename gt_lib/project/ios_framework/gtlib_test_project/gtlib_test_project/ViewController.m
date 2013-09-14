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
    
    [self testGtBuffer];
    NSLog(@"ViewController viewDidLoad");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)testGtArray{
    GtArray_InitLib();
    GtArray_t *array = GtArray_New();
    
    gt_utf8 elementType = 'i';
    
    GtArray_Init(array, &elementType, sizeof(int), 5);
    for(int i=0; i<3 ;i++){
        GtArray_Append(array, &i);
    }

    int nLength = GtArray_GetLength(array);
    for(int i=0; i<nLength ;i++){
        void *element = GtArray_Get(array, i);
        NSLog(@"element %d: %d", i, *((int *)element));
    }
}

- (void)testGtBuffer{
 
}

@end
