#include "FilterUtils.h"
#include <algorithm>
#include "math.h"
#include <cstring>
#include <iostream>

namespace FilterUtils {

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel) {
    // TODO: Task 9 Create buffer to store new image data
    std::cout<<"first"<<std::endl;
    std::vector<RGBA> result;
    result.reserve(width*height);

    // TODO: Task 10 Obtain kernel dimension
    std::cout<<kernel.size()<<std::endl;
    int kernellength = sqrt(kernel.size());
    int kernelwidth = sqrt(kernel.size());
    //double kernellength =  sqrt(kernel.size());

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;

            // TODO: Task 11 Initialize color to accumulate convolution data
            float red_acc=0;
            float green_acc=0;
            float blue_acc=0;

            // TODO: Task 12
            // Iterate over the kernel using the value from task 10
            // Obtain the value at current index of kernel
            // Find correct index in original image data
            // Accumulate the kernel applied to pixel value in color_acc
            //std::cout<<kernellength<<std::endl;

            for(int i =0;i<kernellength;i++){
                for(int j =0;j<kernelwidth;j++){

                    if((r+(i-1))<0||(r+(i-1)>=height)||(c+(j-1)<0)||(c+(j-1)>=width)){
                        continue;
                    }
                    //if((r-(kernelwidth/2)+i)>height || (r-(kernelwidth/2)+i)<0 ||(c-(kernelwidth/2)+j)< 0 ||(c-(kernelwidth/2)+j)>width){
                      //  continue;
                    //}

                    int center = centerIndex+(i-1)*width+(j-1);
                    float currentkernel = kernel[i*kernelwidth+j];
                    float imager = data[center].r/255.0;
                    float imageg = data[center].g/255.0;
                    float imageb = data[center].b/255.0;
                    red_acc += imager*currentkernel;
                    blue_acc += currentkernel*imageb;
                    green_acc += currentkernel *imageg;

                }




            }
            std::cout<<red_acc<<std::endl;

            // TODO: Task 13 Ignore outside bounds


            // TODO: Task 14 Update buffer with accumulated color
            RGBA var={
                REAL2byte(red_acc),REAL2byte(green_acc),REAL2byte(blue_acc)
            };

            result[centerIndex]=var;
            //std::cout<<red_acc<<std::endl;
        }
    }

    // TODO: Task 15 Copy over buffer to canvas data
    memcpy(data,&result[0],(width*height)*sizeof (RGBA));
}

}
