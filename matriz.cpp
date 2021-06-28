//
// Created by henry on 3/06/2021.
//
#include <cmath>
#include <memory>
#include "matriz.h"

Matriz::Matriz(const Matriz &m) {
    for (int i = 0; i < 16; i++) {
        mat[i] = m.mat[i];
    }
}

// res = a cross b;
void Matriz::crossProduct( float *a, float *b, float *res) {
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}

// Normalize a vec3
void Matriz::normalize(float *a) {
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}


void Matriz::LoadIndentity() {
    for (int i=0; i < 16; i++) {
        mat[i] = 0.0;
    }
    mat[0]=mat[5]=mat[10]=mat[15]=1.0;
}
void Matriz::scale(float x, float y, float z) {
    LoadIndentity();
    mat[0] = x;
    mat[5] = y;
    mat[10] = z;
}
void Matriz::rotacion(float x, float y, float z) {
    LoadIndentity();
    mat[0] = cos(x); mat[1] = sin(x);
    mat[4] = -sin(x); mat[5] = cos(x);

    Matriz ry, rz;

    *this = *this * ry * rz;

}
void Matriz::setRotationXMatrix(float theta) {
    LoadIndentity();
    mat[5] = cos(theta);
    mat[6] = sin(theta);
    mat[9] = -sin(theta);
    mat[10] = cos(theta);;
}

// Define a matrix of rotation in Y
void Matriz::setRotationYMatrix(float theta) {
    LoadIndentity();
    mat[0] = cos(theta);
    mat[2] = -sin(theta);
    mat[8] = sin(theta);
    mat[10] = cos(theta);;
}

// Define a matrix of rotation in Z
void Matriz::setRotationZMatrix(float theta) {
    LoadIndentity();
    mat[0] = cos(theta);
    mat[1] = sin(theta);
    mat[4] = -sin(theta);
    mat[5] = cos(theta);;
}



void Matriz::traslacion(float x, float y, float z) {
    LoadIndentity();
    mat[3] = x;
    mat[7] = y;
    mat[11] = z;
}

void Matriz::ProjectionMatrix(float fov, float ratio, float near, float far) {

    //float f = 1.0f / tan(fov * (M_PI / 360.0));
    float n = abs(near);
    float t = n * tan( ANG2RAD * fov / 2 );
    float r = ratio;
    float l = -r;
    float b = -t;
    float f = abs(far);

    LoadIndentity();

    mat[0] = 2*n/(r-l);
    mat[2] = (r+l)/(r-l);
    mat[1 * 4 + 1] = 2*n/(t-b);
    mat[1 * 4 + 2] = (t+b) / (t-b);
    mat[2 * 4 + 2] = (n + f)/(n-f);
    mat[2 * 4 + 3] = 2*n*f/(n-f);
    mat[3 * 3 + 2] = -1;
    mat[3 * 4 + 3] = 0.0f;

    /*mat[0] = f / _ratio;
    mat[1 * 4 + 1] = f;
    mat[2 * 4 + 2] = (_far + _near ) / (_near - _far);
    mat[3 * 4 + 2] = (2.0f * _far * _near) / (_near - _far);
    mat[2 * 4 + 3] = -1.0f;
    mat[3 * 4 + 3] = 0.0f;*/
}

void Matriz::SetCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ,
                       float upx, float upy, float upz) {
    float dir[3], right[3], up[3];
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;

    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);

    crossProduct(dir,up,right);
    normalize(right);

    crossProduct(right,dir,up);
    normalize(up);

    //float aux[16];

    mat[0]  = right[0];
    mat[4]  = right[1];
    mat[8]  = right[2];
    mat[12] = 0.0f;

    mat[1]  = up[0];
    mat[5]  = up[1];
    mat[9]  = up[2];
    mat[13] = 0.0f;

    mat[2]  = dir[0];
    mat[6]  = dir[1];
    mat[10] = dir[2];
    mat[14] =  0.0f;

    mat[3]  = -posX;
    mat[7]  = -posY;
    mat[11] = -posZ;
    mat[15] = 1.0f;

    Matriz aux;
    aux.traslacion(-posX, -posY, -posZ);
    Multiplicacion(aux);
}

// a = a * b;
void Matriz::Multiplicacion(const Matriz &m) {
    float res[16];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += mat[k*4 + i] * m.mat[j*4 + k];
            }
        }
    }
    //memcpy(mat, res, 16 * sizeof(float));
    for (int i = 0; i < 16; i++) {
        mat[i] = res[i];
    }
}
Matriz Matriz::operator*(const Matriz &m) {
    Matriz p;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            p.mat[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                p.mat[j*4 + i] += mat[k*4 + i] * m.mat[j*4 + k];
            }
        }
    }
    return p;
}


float* Matriz::Multiplicacion(float *vertice) {
    float *res = new float[4];
    float *v = new float[4];
    v[0] = vertice[0];
    v[1] = vertice[1];
    v[2] = vertice[2];
    v[3] = 1.0f;

    res[0] = mat[0]   *v[0] + mat[1]   *v[1] + mat[2]   *v[2] + mat[3]   *v[3];
    res[1] = mat[0+4] *v[0] + mat[1+4] *v[1] + mat[2+4] *v[2] + mat[3+4] *v[3];
    res[2] = mat[0+8] *v[0] + mat[1+8] *v[1] + mat[2+8] *v[2] + mat[3+8] *v[3];
    res[3] = mat[0+12]*v[0] + mat[1+12]*v[1] + mat[2+12]*v[2] + mat[3+12]*v[3];
    return res;
}



