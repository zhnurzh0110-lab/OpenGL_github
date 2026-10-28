#pragma once
#include "../glm.hpp"
namespace glm {
inline mat4 translate(const mat4&m,vec3 v){ mat4 r=m; r.c[3]=m.c[0]*v.x+m.c[1]*v.y+m.c[2]*v.z+m.c[3]; return r; }
inline mat4 scale(const mat4&m,vec3 v){ mat4 r; r.c[0]=m.c[0]*v.x; r.c[1]=m.c[1]*v.y; r.c[2]=m.c[2]*v.z; r.c[3]=m.c[3]; return r; }
inline mat4 rotate(const mat4&m,float angle,vec3 axis_){
  float c=std::cos(angle), s=std::sin(angle); vec3 a=normalize(axis_); vec3 t=a*(1-c);
  float R[3][3];
  R[0][0]=c+t.x*a.x; R[0][1]=t.x*a.y+s*a.z; R[0][2]=t.x*a.z-s*a.y;
  R[1][0]=t.y*a.x-s*a.z; R[1][1]=c+t.y*a.y; R[1][2]=t.y*a.z+s*a.x;
  R[2][0]=t.z*a.x+s*a.y; R[2][1]=t.z*a.y-s*a.x; R[2][2]=c+t.z*a.z;
  mat4 r; for(int j=0;j<3;++j) r.c[j]=m.c[0]*R[j][0]+m.c[1]*R[j][1]+m.c[2]*R[j][2]; r.c[3]=m.c[3]; return r; }
inline mat4 lookAt(vec3 eye,vec3 center,vec3 up){
  vec3 f=normalize(center-eye), s=normalize(cross(f,up)), u=cross(s,f); mat4 r(1.0f);
  r.c[0].x=s.x; r.c[1].x=s.y; r.c[2].x=s.z; r.c[0].y=u.x; r.c[1].y=u.y; r.c[2].y=u.z;
  r.c[0].z=-f.x; r.c[1].z=-f.y; r.c[2].z=-f.z; r.c[3].x=-dot(s,eye); r.c[3].y=-dot(u,eye); r.c[3].z=dot(f,eye); return r; }
inline mat4 perspective(float fovy,float aspect,float zn,float zf){
  float th=std::tan(fovy/2); mat4 r; r.c[0].x=1/(aspect*th); r.c[1].y=1/th; r.c[2].z=-(zf+zn)/(zf-zn); r.c[2].w=-1; r.c[3].z=-(2*zf*zn)/(zf-zn); return r; }
}
