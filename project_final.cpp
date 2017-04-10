#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <sstream>
#include <iostream>
//#include <cvcam.h>
#define TH1 20
#define alpha 1
using namespace std;
IplImage* cvClone_func(IplImage *source,IplImage *target);
IplImage* merge(IplImage *item,IplImage *cv_frame,int x,int y);
IplImage* merge2(IplImage *item,IplImage *cv_frame,int x,int y);
int main() {
	CvCapture *capture;
    IplImage *origin,*game,*detect,*move,*feed,*animal,*last,*lose,*start,*intro,*back,*fence;
    // Create Image
	capture=cvCreateCameraCapture( 0 );
	origin = cvQueryFrame(capture);
	game = cvCreateImage(cvSize(origin->width,origin->height), IPL_DEPTH_8U, 3);
	detect = cvCreateImage(cvSize(origin->width,origin->height), IPL_DEPTH_8U, 3);
	move = cvCreateImage(cvSize(origin->width,origin->height), IPL_DEPTH_8U, 3);
	CvFont font;
	cvInitFont(&font, CV_FONT_VECTOR0,1,1,0,3,8);
    int stage=1,animal_x=0,x1,y1,score=5;
    bool isReleased=false;
	const char* score_str;
    feed=cvLoadImage("feed.png");
	last=cvLoadImage("last.png");
	lose=cvLoadImage("lose.png");
	start=cvLoadImage("start.png");
	intro=cvLoadImage("intro.png");
	fence=cvLoadImage("fence.png");
	cvNamedWindow("origin",1);
    cvNamedWindow("game",1);
	// Color Detect
	float refBlue=0,refGreen=0,refRed=0,refCounter=0,preprocessTime=60;
	while (preprocessTime > 0) {
		origin = cvQueryFrame(capture);
		cvFlip(origin,origin,1);
		cvRectangle(origin, cvPoint(300,200), cvPoint(318,220), cvScalar(0,0,255), 2);
		for (int y=205; y<215; y++) {
			uchar* ptr=(uchar*) (origin->imageData +y*origin->widthStep);
			for (int x=305; x<315; x++) {
				refBlue+=ptr[3*x];
				refGreen+=ptr[3*x+1];
				refRed+=ptr[3*x+2];
				refCounter++;
			}
        }
 		cvWaitKey(100);
		preprocessTime--;
		cvShowImage("origin", origin);
	}
	cvDestroyWindow("origin");
	refBlue=refBlue/refCounter;
	refGreen=refGreen/refCounter;
	refRed=refRed/refCounter;
	//start 
	cvShowImage("game" , start);
	cvWaitKey(0);
	cvShowImage("game" , intro);
	cvWaitKey(0);	
	while(1) {
		origin = cvQueryFrame(capture);
		cvFlip(origin,origin,1);
		game = cvClone_func(origin,game);
		detect = cvClone_func(origin,detect);
		move = cvClone_func(origin,move);
        // game background
        for (int y=0; y<game->height; y++) {
			uchar* ptr=(uchar*) (game->imageData +y*game->widthStep);
			for (int x=0; x<game->width; x++) {
				ptr[3*x]=0;
				ptr[3*x+1]=0;
				ptr[3*x+2]=0;
			}
        }

		// merge animal into game at (x,y)
		switch(stage) {
			case 1:
				animal=cvLoadImage("cat1.png"); 
				back=cvLoadImage("back1.jpg");
				break;
			case 2:
				animal=cvLoadImage("fox1.png"); 
				back=cvLoadImage("back2.png");
				break;
			case 3:
				animal=cvLoadImage("dog8.png"); 
				back=cvLoadImage("back3.png");
				break;
			case 4:
				animal=cvLoadImage("rabbit2.png"); 
				back=cvLoadImage("back4.png");
				break;
			case 5:
				animal=cvLoadImage("doge1.png"); 
				back=cvLoadImage("back5.png");
				break;
			case 6:
				animal=cvLoadImage("cat9.png"); 
				back=cvLoadImage("back11.png");
				break;
			case 7:
				animal=cvLoadImage("dog10.png"); 
				back=cvLoadImage("back14.png");
				break;
			case 8:
				animal=cvLoadImage("rabbit4.png"); 
				back=cvLoadImage("back18.png");
				break;
			case 9:
				animal=cvLoadImage("cat8.png"); 
				back=cvLoadImage("back12.png");
				break;	
			case 10:
				animal=cvLoadImage("redpanda1.png"); 
				back=cvLoadImage("back6.png");
				break;
			case 11:
				animal=cvLoadImage("penguin1.png"); 
				back=cvLoadImage("back7.png");
				break;
			case 12:
				animal=cvLoadImage("cat4.png"); 
				back=cvLoadImage("back13.jpg");
				break;
			case 13:
				animal=cvLoadImage("dog1.png"); 
				back=cvLoadImage("back15.png");
				break;
			case 14:
				animal=cvLoadImage("unicorn1.png"); 
				back=cvLoadImage("back8.png");
				break;
			case 15:
				animal=cvLoadImage("monkey1.png"); 
				back=cvLoadImage("back9.png");
				break;
			case 16:
				animal=cvLoadImage("fish1.png"); 
				back=cvLoadImage("back10.png");
				break;	
			case 17:
				animal=cvLoadImage("dog9.png"); 
				back=cvLoadImage("back16.png");
				break;
			case 18:
				animal=cvLoadImage("fox2.png"); 
				back=cvLoadImage("back17.png");
				break;
			default:
				break;
		}
		game=merge2(back,game,0,0);
		game=merge(fence,game,0,200);
		game=merge(animal,game,animal_x,0);
        if(animal_x<=635) animal_x+=10;
		if(animal_x>=635){
			while(animal_x>=5) animal_x-=10;
		}
		//score
        stringstream ss;
		ss<<score;
		score_str = ss.str().c_str();
        cvPutText(game,score_str,cvPoint(30,30),&font,CV_RGB(255,0,0));
        // virtual mouse
		int refX=0,refY=0,xyCounter=0;
		for (int y=0; y<detect->height; y++) {
			uchar* ptr=(uchar*) (detect->imageData +y*detect->widthStep);
			for (int x=0; x<detect->width; x++) {
				float D= (ptr[3*x] - refBlue) *  (ptr[3*x] - refBlue) +
				         (ptr[3*x+1] - refGreen) * (ptr[3*x+1] -refGreen) +
				         (ptr[3*x+2] - refRed) * (ptr[3*x+2] - refRed);
				if (sqrt(D) < TH1) {
					ptr[3*x]=int(refBlue);
					ptr[3*x+1]=int(refGreen);
					ptr[3*x+2]=int(refRed);
					xyCounter++;
					refX+=x;
					refY+=y;
				}
			}
		}
		if (xyCounter > 0) {
			refX = refX/xyCounter;
			refY = refY/xyCounter;
		} 
        else {
			refX=-1;
			refY=-1;
		}	    
		// show virtual mouse on detect & merge feed into move
		cvCircle(detect,cvPoint(refX,refY),5,CV_RGB(0,255,255),3,CV_AA,0);
		if(refY<200) isReleased=true;      
		if (refX >=0 && refY >=0&&isReleased==false) {
			x1=refX;
			y1=refY;
		}
		else if (refX >=0 && refY >=0&&isReleased==true) {
			y1-=10;
			if(y1<=10){
				isReleased=false;
				score-=5;
			}
		}		
		if(y1<animal->height&&x1>animal_x-5&&x1<animal_x+animal->width){
			isReleased=false;
			stage++;
			score+=10;                 
		}
				
	    if ( (feed->height + y1) > move->height )
            y1 = move->height - feed->height - 1;            
		move=merge(feed,move,x1,y1);
		game=merge(feed,game,x1,y1);
		// show image
		cvShowImage("game", game);
        cvShowImage("detect", detect);
        //result
		if(score>=100){
			cvDestroyWindow("game");
			cvDestroyWindow("origin");
    		cvDestroyWindow("detect");
			cvDestroyWindow("move");
			cvShowImage("game" , last);			
			cvWaitKey(0);
		}
		if(score<0){
			cvDestroyWindow("game");
			cvDestroyWindow("origin");
    		cvDestroyWindow("detect");
			cvDestroyWindow("move");
			cvShowImage("game" , lose);	
			cvWaitKey(0);
		}
		cvWaitKey(100);
	}
	cvReleaseCapture(&capture);
	cvReleaseImage(&origin);
    cvReleaseImage(&game);
	cvReleaseImage(&origin);
    cvReleaseImage(&detect);
	cvReleaseImage(&move);	
	return 0;
}
// function
IplImage* cvClone_func(IplImage *s,IplImage *t){    
    for (int row=0; row<s->height; row++) {
		uchar* ptr_s=(uchar*) (s->imageData +row*s->widthStep);
		uchar* ptr_t=(uchar*) (t->imageData +row*t->widthStep);
		for (int col=0; col<s->width; col++) {
		    ptr_t[3*col]=ptr_s[3*col];
			ptr_t[3*col+1]=ptr_s[3*col+1];
			ptr_t[3*col+2]=ptr_s[3*col+2];
		}
	}
	return t;
}
IplImage* merge(IplImage *item,IplImage *ori,int col,int row){
    for (int y=0; y<item->height; y++) {
		uchar* ptr1=(uchar*) (item->imageData +y*item->widthStep);
		uchar* ptr2=(uchar*) (ori->imageData +(y+row)*ori->widthStep);
		for (int x=0; x<item->width; x++) {
			if(ptr1[3*x]<250&&ptr1[3*x+1]<250&&ptr1[3*x+2]<250){
				ptr2[3*(x+col)]=alpha*ptr1[3*(x)] + (1-alpha)*ptr2[3*(x+col)];
				ptr2[3*(x+col)+1]=alpha*ptr1[3*(x)+1] + (1-alpha)*ptr2[3*(x+col)+1];
				ptr2[3*(x+col)+2]=alpha*ptr1[3*(x)+2] + (1-alpha)*ptr2[3*(x+col)+2];
			}
		}
	}
	return ori;
}
IplImage* merge2(IplImage *item,IplImage *ori,int col,int row){
    for (int y=0; y<item->height; y++) {
		uchar* ptr3=(uchar*) (item->imageData +y*item->widthStep);
		uchar* ptr4=(uchar*) (ori->imageData +(y+row)*ori->widthStep);
		for (int x=0; x<item->width; x++) {
			ptr4[3*(x+col)]=alpha*ptr3[3*(x)] + (1-alpha)*ptr4[3*(x+col)];
			ptr4[3*(x+col)+1]=alpha*ptr3[3*(x)+1] + (1-alpha)*ptr4[3*(x+col)+1];
			ptr4[3*(x+col)+2]=alpha*ptr3[3*(x)+2] + (1-alpha)*ptr4[3*(x+col)+2];
		}
	}
	return ori;
}
