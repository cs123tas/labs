#include "FilterShift.h"
#include "FilterUtils.h"
#include "iostream"
FilterShift::FilterShift(ShiftDirection direction):
    m_shiftDir(direction)
{

    // TODO: Task 19 Initialize kernel for shift
    std::vector<float> m_kernel;





}


FilterShift::~FilterShift()
{
}

std::vector<float> FilterShift::makeKernel(ShiftDirection direction){
    /*td::cout<<m_shiftDir<<std::endl;*/
    if(direction==SHIFT_RIGHT){

        m_kernel ={
            0,0,0,1,0,0,0,0,0
        };

    }
    if (direction==SHIFT_LEFT){
        m_kernel ={
            0,0,0,0,0,1,0,0,0
        };

    }
    return m_kernel;
}
void FilterShift::apply(Canvas2D *canvas) {
    // TODO: Task 18 Call convolve function from superclass
    m_kernel = makeKernel(m_shiftDir);
//    std::cout<<"getit"<<std::endl;
//    std::cout<<m_kernel.size()<<std::endl;
//    std::cout<<"right"<<std::endl;
//    for (int x=0; x<sqrt(m_kernel.size());x++){
//        for (int y=0; y<sqrt(m_kernel.size());y++){
//            //printf("%d", m_kernel[(int)(x*sqrt(m_kernel.size())+y)]);
//            std::cout<<m_kernel[(int)(x*sqrt(m_kernel.size())+y)]<<std::endl;
//        }
//    }

    //std::cout<<m_kernel<<std::endl;
    FilterUtils::Convolve2D(canvas->data(),canvas->width(),canvas->height(), m_kernel);
}
