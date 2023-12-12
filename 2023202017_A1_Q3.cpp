#include <opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
Mat seamRemoveHorizontally(Mat& image,Mat& energyfunction,int* cost,int height,int width){
    int ei=0;
    int k=0;
    Mat newimage(height-1,width,CV_8UC3);
    Mat newenergyfunction(height-1,width,CV_64F);
    for(int j=0;j<width;j++){  
        int si=0;     
        for(int i=0;i<height;i++){
            if(i<cost[k]){
                newimage.at<Vec3b>(si, ei) = image.at<Vec3b>(i, j);
                newenergyfunction.at<double>(si,ei)=energyfunction.at<double>(i,j);
                si++;
            }
            if(i>cost[k]){
                newimage.at<Vec3b>(si, ei) = image.at<Vec3b>(i, j);
                newenergyfunction.at<double>(si,ei)=energyfunction.at<double>(i,j);
                si++;
            }
        }
        ei++;
        k++;
    }
   //energyfunction=newenergyfunction.clone();
   return newimage;
}
//to identify the seam horizontally
int* seamIdentificationHorizontally(Mat& energyfunction,int* cost,int height,int width){
    //copy as it is row 1
    double** dp=new double*[height];
    for(int i=0;i<height;i++){
        dp[i]=new double[width];
    }
    //start from col 1
    for(int i=0;i<height;i++){
        dp[i][0]=energyfunction.at<double>(i,0);
    }
    for(int j=1;j<width;j++){
        for(int i=0;i<height;i++){
            if(i>0 && i<height-1){
                //you are at middle
                dp[i][j]=energyfunction.at<double>(i,j)+min(dp[i-1][j-1],min(dp[i][j-1],dp[i+1][j-1]));
            }
            else if(i==0){
                dp[i][j]=energyfunction.at<double>(i,j)+min(dp[i][j-1],dp[i+1][j-1]);
            }
            else if(i==height-1){
                dp[i][j]=energyfunction.at<double>(i,j)+min(dp[i][j-1],dp[i-1][j-1]);
            }
        }
    }

    //do backtracking
    int* newcost=new int[width];
    double mini=FLT_MAX;
    int index=-1;
    //starting from last column
    for(int i=0;i<height;i++){
       if(mini>dp[i][width-1]){
          mini=dp[i][width-1];
          index=i;
       }
    }
    newcost[width-1]=index;
    int lastIndex=index;
    for(int j=width-2;j>=0;j--){
        mini=FLT_MAX;
        for(int i=height-1;i>=0;i--){
            if(i==lastIndex || i==lastIndex-1 || i==lastIndex+1){
                if(mini>dp[i][j]){
                    mini=dp[i][j];
                    index=i;
                }
            }
        }
        newcost[j]=index;
        lastIndex=index;
    }
    return newcost;

}

Mat seamRemoveVertically(Mat& image,Mat energyfunction,int* cost,int height,int width){
    int si=0;
    int k=0;
    Mat newimage(height,width-1,CV_8UC3);
    for(int i=0;i<height;i++){  
        int ei=0;     
        for(int j=0;j<width;j++){
            if(j<cost[k]){
                newimage.at<Vec3b>(si, ei) = image.at<Vec3b>(i, j);
                ei++;
            }
            if(j>cost[k]){
                newimage.at<Vec3b>(si, ei) = image.at<Vec3b>(i, j);
                ei++;
            }
        }
        si++;
        k++;
    }
   // energyfunction=newenergyfunction.clone();
   return newimage;
}

//to identify the seam vertically 
int* seamIdentificationVertically(Mat energyfunction,int* cost,int height,int width){
    //copy as it is row 1
    double** dp=new double*[height];
    for(int i=0;i<height;i++){
        dp[i]=new double[width];
    }
    //copying the value of energy as it is
    for(int j=0;j<width;j++){
        dp[0][j]=energyfunction.at<double>(0,j);
    }
    //start from row 1
    for(int i=1;i<height;i++){
        for(int j=0;j<width;j++){
            if(j>0 && j<width-1){
                //you are at middle
                dp[i][j]=energyfunction.at<double>(i,j)+min(dp[i-1][j],min(dp[i-1][j+1],dp[i-1][j-1]));     
            }
            else if(j==0){
                dp[i][j]=energyfunction.at<double>(i,j)+min(dp[i-1][j],dp[i-1][j+1]);
            }
            else if(j==width-1){
                dp[i][j]=energyfunction.at<double>(i,j)+min(dp[i-1][j-1],dp[i-1][j]);
            }
        }
    }

    //do backtracking
    int* newcost=new int[height];
    double mini=FLT_MAX;
    int index=-1;
    //starting from last row
    for(int j=0;j<width;j++){
       if(mini>dp[height-1][j]){
          mini=dp[height-1][j];
          index=j;
       }
    }
    newcost[height-1]=index;
    int lastIndex=index;
    for(int i=height-2;i>=0;i--){
        mini=FLT_MAX;
        for(int j=width-1;j>=0;j--){
            if(j==lastIndex || j==lastIndex-1 || j==lastIndex+1){
                if(mini>dp[i][j]){
                    mini=dp[i][j];
                    index=j;
                }
            }
        }
        newcost[i]=index;
        lastIndex=index;
    }
    return newcost;
}

double helpEnergyFunction(const cv::Mat& image, int x, int y) {
    int xleft,xright;
    int yup,ydown;
    if(x==0){
        xleft=image.cols-1;
        xright=x+1;
    }
    else if(x==image.cols-1){
        xleft=x-1;
        xright=0;
    }
    else{
        xleft=x-1;
        xright=x+1;
    }
    if(y==0){
        yup=image.rows-1;
        ydown=y+1;
    }
    else if(y==image.rows-1){
        yup=y-1;
        ydown=0;
    }
    else{
        yup=y-1;
        ydown=y+1;
    }
    //coordinate when to find the left one then x of that will be x_left and y will be y
    Vec3b pAtXLeft = image.at<Vec3b>(y, xleft);
     //coordinate when to find the right one then x of that will be x_right and y will be y
    Vec3b pAtXRight = image.at<Vec3b>(y, xright);
     //coordinate when to find the upper one then x of that will be x and y will be y_up
    Vec3b pAtYUpper = image.at<Vec3b>(yup, x);
     //coordinate when to find the left one then x of that will be x and y will be y_down
    Vec3b pAtYDown= image.at<Vec3b>(ydown, x);

    //now to find the xgradient formula is sqrt(x_left^2 + x_right^2);

    //first finding the RGB 
    double xgrad=0;
    for(int i=2;i>=0;i--){
        //first R -> G -> B 
        double diff= abs(pAtXLeft[i]-pAtXRight[i]);
        xgrad+=(diff*diff); 
    }
    double ygrad=0;
    //RGB
    for(int i=2;i>=0;i--){
        //first R -> G -> B 
        double diff= abs(pAtYUpper[i]-pAtYDown[i]);
        ygrad+=(diff*diff); 
    }
    double gradient=sqrt(xgrad+ygrad);
    return gradient;
}
Mat energySingleValue2(Mat& image){
    // Calculate the energy for each pixel
    Mat energyMap(image.rows, image.cols, CV_64F);
    for (int j = 0; j < image.rows; j++) {
        for (int i = 0; i < image.cols; i++) {
            energyMap.at<double>(j, i) = helpEnergyFunction(image, i, j);
        }
    }
    return energyMap;
}
int main() {
    string path;
    cin>>path;
    int newHeight;
    int newWidth;
    cin>>newHeight>>newWidth;
    // /Users/suyash9698/Desktop/cpp/opencv-setter/ + path
    //taking this as a path only
    Mat image = imread(path);
    if (image.empty()) {
        cout << "Unable to load the image...." <<endl;
        exit(1);
    }
    if(newHeight>image.rows-10 || newWidth>image.cols-10 || newHeight<=0 || newWidth<=0){
        cout<<"Please enter NewHeight input in the range of "<<"0"<<" "<<(image.rows-10)<<endl;
        cout<<"Please enter NewWidth input in the range of "<<"0"<<" "<<(image.cols-10)<<endl;
        exit(1);
    }
    Mat energyfunction=energySingleValue2(image);
    int* cost;
    Mat newimage;
    if(newHeight==image.rows && newWidth==image.cols){
        imwrite("output_image.jpg", image);
        exit(1);
    }
    int toRemoveWidth=image.cols-newWidth;
    int toRemoveHeight=image.rows-newHeight;
    //removing width
    for(int i=0;i<1 && toRemoveWidth>0;i++){
        cost=seamIdentificationVertically(energyfunction,cost,image.rows,image.cols);
        newimage=seamRemoveVertically(image,energyfunction,cost,image.rows,image.cols);
    }
    for(int i=0;i<toRemoveWidth-1;i++){
        energyfunction=energySingleValue2(newimage);
        cost=seamIdentificationVertically(energyfunction,cost,newimage.rows,newimage.cols);
        newimage=seamRemoveVertically(newimage,energyfunction,cost,newimage.rows,newimage.cols);
    }
    //removing height
    for(int i=0;i<toRemoveHeight;i++){
        energyfunction=energySingleValue2(newimage);
        cost=seamIdentificationHorizontally(energyfunction,cost,newimage.rows,newimage.cols);
        newimage=seamRemoveHorizontally(newimage,energyfunction,cost,newimage.rows,newimage.cols);
    }

    
     namedWindow("NewImage", WINDOW_AUTOSIZE);
     //imshow("NewImage",newimage);
     imwrite("output_image.jpg", newimage);
    //   int press=waitKey(0);
    //  //press enter to exit
    //  if(press==13){
    //     destroyWindow("NewImage");
    //  }
    //  //or press escape key
    //  if(press==27){
    //     destroyWindow("NewImage");
    //  }
}

