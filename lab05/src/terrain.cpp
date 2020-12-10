#include "terrain.h"
#include <iostream>
#include <math.h>
#include "gl/shaders/ShaderAttribLocations.h"

Terrain::Terrain() : m_numRows(100), m_numCols(m_numRows)
{
}


/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and column.
 */
float Terrain::randValue(int row, int col) {
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}

float Terrain::getNoiseValue(int row, int col, float octave, float frequency, float persistence) {
    float newRow = row/octave;
    float newCol = col/octave;
    float fracRow = glm::fract(newRow);
    float fracCol = glm::fract(newCol);
    float floorRow = glm::floor(newRow);
    float floorCol = glm::floor(newCol);
    //position.y = randValue(newRow,newCol);

    float up = glm::mix(randValue(floorRow, floorCol), randValue(floorRow, floorCol + 1), fracCol * fracCol * (3 - 2 *fracCol));
    float down = glm::mix(randValue(floorRow + 1, floorCol), randValue(floorRow + 1, floorCol + 1), fracCol * fracCol * (3 - 2 *fracCol));
    float x =fracRow * fracRow * (3 - 2 *fracRow);
    float result = glm::mix(up,down,x);

    return result;
}

/**
 * Returns the object-space position for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getPosition(int row, int col) {
    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.y = 0;
    position.z = 10 * col/m_numCols - 5;
//    position.x = 20 * row/m_numRows - 10;
//    position.y = 0;
//    position.z = 20 * col/m_numCols - 10;

    // TODO: Adjust position.y using value noise.
    //float oct = glm::pow(2.0,1.0);
    float oct = 10.0;
    float fact = 2.0;
    float scale = 1.0;
    for(int x=0;x<5;x++){
        float result = getNoiseValue(row,col, oct,0,0);
        float lower = 2.00;
        result = glm::pow(result,lower);
        position.y += result;
//        oct = glm::pow((float)2.0,fact);
//        fact++;
        oct = oct*fact;
        scale = scale/2.0;
    }
    //hi
    //if(col>50 &&row>50){
        if(col>150 &&row>150){
        position.y=.25;
       // position.y = .45;
       //     position.y = sqrt(position.y)/3;
    }
    if(col>0 &&col>40 &&row<20){
       position.y=position.y/3;
    }
    return position;
}


/**
 * Returns the normal vector for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getNormal(int row, int col) {
    // TODO: Compute the normal at the given row and column using the positions of the
    //       neighboring vertices.
    glm::vec3 positions[9];
   positions[0] = getPosition(row, col);
   positions[1] = getPosition(row , col + 1);
   positions[2] = getPosition(row - 1, col + 1);
   positions[3] = getPosition(row - 1, col);
   positions[4] = getPosition(row - 1, col - 1);
   positions[5] = getPosition(row, col - 1);
   positions[6] = getPosition(row + 1, col - 1);
   positions[7] = getPosition(row + 1, col);
   positions[8] = getPosition(row + 1, col + 1);

   glm::vec3 vectors[8];

   for (int i = 0; i < 8; i++) {
       vectors[i] = positions[0] - positions[i + 1];
   }

   glm::vec3 normals[8];

   for (int i = 0; i < 7; i++) {
       vectors[i] = glm::cross(positions[i + 1], positions[i]);
   }
   vectors[7] = glm::cross(positions[0], positions[7]);

   float x;
   float y;
   float z;

   for (int i = 0; i < 8; i ++) {
       x += vectors[i].x;
       y += vectors[i].y;
       z += vectors[i].z;
   }

   x = x / 8.0;
   y = y / 8.0;
   z = z / 8.0;
   glm::vec3 result ={x,y,z};

   return glm::normalize(result);
//    return glm::vec3(0, 1, 0);
}



struct terrainColor {
    terrainColor(float _height, glm::vec3 _color) {
        height = _height;
        color = _color;
    };
    float height;
    glm::vec3 color;
};

glm::vec3 Terrain::generate_biome(glm::vec3 position, int row, int col) {
    std::vector<glm::vec3> colors;
    std::vector<terrainColor> biomeColors;

    glm::vec3 color = glm::vec3(255/255.0, 255/255.0, 255/255.0);


    biomeColors.push_back(terrainColor(.15, glm::vec3(101/255.0, 67/255.0, 33/255.0)));   // brown for mountain
    biomeColors.push_back(terrainColor(.15, glm::vec3(255/255.0, 255/255.0, 255/255.0)));   // snow for mountain


    if(col>0 &&col>40 &&row<20){
            color = glm::vec3(156/255.0, 211/255.0, 219/255.0);//blue
        }
//    else if(position.y<4.1 &&position.y>.45){
    else if(position.y<.9){
        //color = biomeColors[0].color;
        color = glm::vec3(101/255.0, 67/255.0, 33/255.0);//brown
    }
    else if(position.y>.9){
  //  else if(position.y>4.1){
//        color = biomeColors[1].color;
        color= glm::vec3(255/255.0, 255/255.0, 255/255.0);//white
    }
//    if(col>50 &&row>50){

//        color = glm::vec3(132/255.0, 192/255.0, 17/255.0);//green
//    }
//    else if((position.y<=.45)){
//        //color = biomeColors[0].color;

//        color = glm::vec3(132/255.0, 192/255.0, 17/255.0);//green


//    }
//    else if((position.y<=.45)&&!(col>150 &&row>150)){
//        //color = biomeColors[0].color;
//        color = glm::vec3(1/255.0, 68/255.0, 33/255.0);//green

//    }
    //if(col>50 &&row>50){
    if(col>150 &&row>150){

       color = glm::vec3(132/255.0, 192/255.0, 17/255.0);//green
}
    //colors.push_back(color);
    return color;
}

/**
 * Initializes the terrain by storing positions and normals in a vertex buffer.
 */
void Terrain::init(GLuint m_program) {
    // TODO: Change from GL_LINE to GL_FILL in order to render full triangles instead of wireframe.

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    std::vector<glm::vec3> colors;

    // Initializes a grid of vertices using triangle strips.
    int numVertices = (m_numRows - 1) * (2 * m_numCols + 2);

//    std::vector<float> data(9 * numVertices);
    std::vector<float> data(18 * numVertices);
    int index = 0;
    int offset = 0;
   // for(int i =0; i< 3; i++){
    for (int row = 0; row < m_numRows - 1; row++) {
        for (int col = m_numCols - 1; col >= 0; col--) {

            data[index++] = getPosition(row, col).x;
            data[index++] = getPosition(row, col).y;
            data[index++] = getPosition(row, col).z;
            data[index++] = getNormal  (row, col).x;
            data[index++] = getNormal  (row, col).y;
            data[index++] = getNormal  (row, col).z;

            data[index++] = generate_biome(getPosition(row, col), row, col).x;
            data[index++] = generate_biome(getPosition(row, col), row, col).y;
            data[index++] = generate_biome(getPosition(row, col), row, col).z;
//            data[index++] =0.0;
//            data[index++] =0.0;
            data[index++] = getPosition(row, col).x;
            data[index++] = getPosition(row, col).y;
            data[index++] = getPosition(row + 1, col).x;
            data[index++] = getPosition(row + 1, col).y;
            data[index++] = getPosition(row + 1, col).z;
            data[index++] = getNormal  (row + 1, col).x;
            data[index++] = getNormal  (row + 1, col).y;
            data[index++] = getNormal  (row + 1, col).z;
//            data[index++] =0.0;
//            data[index++] =1.0;
            data[index++] = generate_biome(getPosition(row+1, col),row + 1, col).x;
            data[index++] = generate_biome(getPosition(row+1, col),row + 1, col).y;
            data[index++] = generate_biome(getPosition(row+1, col),row + 1, col).z;
            data[index++] = getPosition(row + 1, col).x;
            data[index++] = getPosition(row + 1, col).y;

        }
        data[index++] = getPosition(row + 1, 0).x;
        data[index++] = getPosition(row + 1, 0).y;
        data[index++] = getPosition(row + 1, 0).z;
        data[index++] = getNormal  (row + 1, 0).x;
        data[index++] = getNormal  (row + 1, 0).y;
        data[index++] = getNormal  (row + 1, 0).z;
        data[index++] = generate_biome(getPosition(row+1, 0),row + 1, 0).x;
        data[index++] = generate_biome(getPosition(row+1, 0),row + 1, 0).y;
        data[index++] = generate_biome(getPosition(row+1, 0),row + 1, 0).z;
//        data[index++] =1.0;
//        data[index++] =1.0;
        data[index++] = getPosition(row + 1, 0).x;
        data[index++] = getPosition(row + 1, 0).y;
        data[index++] = getPosition(row + 1, m_numCols - 1).x;
        data[index++] = getPosition(row + 1, m_numCols - 1).y;
        data[index++] = getPosition(row + 1, m_numCols - 1).z;
        data[index++] = getNormal  (row + 1, m_numCols - 1).x;
        data[index++] = getNormal  (row + 1, m_numCols - 1).y;
        data[index++] = getNormal  (row + 1, m_numCols - 1).z;
//        data[index++] =1.0;
//        data[index++] =0.0;
        data[index++] = generate_biome(getPosition(row+1, m_numCols - 1),row + 1, m_numCols - 1).x;
        data[index++] = generate_biome(getPosition(row+1, m_numCols - 1),row + 1, m_numCols - 1).y;
        data[index++] = generate_biome(getPosition(row+1, m_numCols - 1),row + 1, m_numCols - 1).z;
        data[index++] = getPosition(row + 1, m_numCols - 1).x;
        data[index++] = getPosition(row + 1, m_numCols - 1).y;


    }
 //   offset += 100;

 //}


    // Initialize OpenGLShape.
    m_shape = std::make_unique<OpenGLShape>();
    //m_shape->setVertexData(&data[0][0], data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, numVertices);
    m_shape->setVertexData(&data[0], data.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, numVertices);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::COLOR, 3, sizeof(glm::vec3)*2, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::TEXCOORD0, 2, sizeof(glm::vec3)*3,VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();

    // Smart pointer!
//    m_square = std::make_unique<OpenGLShape>();

//    static constexpr int kFloatsPerVertex = 8;
//    std::vector<float> coordinates = {
//        -0.5f,  0.5f,  0.0f, 1, 0, 0, 0, 0,
//        -0.5f, -0.5f,  0.0f, 0, 1, 1, 0, 1,
//         0.5f,  0.5f,  0.0f, 1, 0, 1, 1, 0,
//         0.5f, -0.5f,  0.0f, 1, 1, 0, 1, 1
//    };
//    m_square->setVertexData(coordinates.data(),
//                            coordinates.size(),
//                            VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP,
//                            coordinates.size() / kFloatsPerVertex);


    // TODO: Interleave positions and colors in the array used to intialize m_square (Task 10)

    // TODO: Don't forget to add the color attribute similar to how you did for the position (Task 10)

    // TODO: Interleave UV-coordinates along with positions and colors in your VBO (Task 14)
//    m_square->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    m_square->setAttribute(ShaderAttrib::COLOR, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    m_square->setAttribute(ShaderAttrib::TEXCOORD0, 2, 24, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    m_square->buildVAO();
}


/**
 * Draws the terrain.
 */
void Terrain::draw()
{
    m_shape->draw();
   // m_square->draw();
}
