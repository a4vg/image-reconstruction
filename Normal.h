//
// Created by henry on 14/06/2021.
//

#ifndef CG4_PRISMA_NORMAL_H
#define CG4_PRISMA_NORMAL_H

#include <cmath>

class Normal{
public:
    float x, y, z;

    Normal() { x=y=z=0; }
    Normal(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Normal(const Normal &n) {
        x = n.x; y = n.y; z = n.z;
    }

    Normal operator+(const Normal &n) {
        Normal s(x + n.x, y + n.y, z + n.z);
        s.normalize();
        return s;
    }
    Normal operator/(float num) {
        Normal d(x/num, y/num, z/num);
        d.normalize();
        return d;
    }

    void normalize() {
        float mag = sqrt(x*x +  y*y + z*z);
        x /= mag;
        y /= mag;
        z /= mag;
    }
};

#endif //CG4_PRISMA_NORMAL_H
