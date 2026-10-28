#pragma once
#include <cmath>
#include <algorithm>
#include <cstddef>
namespace glm {
struct vec4;
struct vec2 { float x, y; vec2():x(0),y(0){} explicit vec2(float a):x(a),y(a){} vec2(float a,float b):x(a),y(b){} };
struct vec3 {
  union{float x;float r;}; union{float y;float g;}; union{float z;float b;};
  vec3():x(0),y(0),z(0){} explicit vec3(float a):x(a),y(a),z(a){} vec3(float a,float b_,float c):x(a),y(b_),z(c){}
  template<class X,class Y,class Z> vec3(X a,Y b_,Z c):x((float)a),y((float)b_),z((float)c){}
  vec3& operator+=(const vec3&o){x+=o.x;y+=o.y;z+=o.z;return *this;}
  vec3& operator-=(const vec3&o){x-=o.x;y-=o.y;z-=o.z;return *this;}
  vec3& operator+=(float o){x+=o;y+=o;z+=o;return *this;}
  vec3& operator*=(float o){x*=o;y*=o;z*=o;return *this;}
};
struct vec4 {
  float x,y,z,w; vec4():x(0),y(0),z(0),w(0){} vec4(float a,float b,float c,float d):x(a),y(b),z(c),w(d){}
  vec4(const vec3&v,float d):x(v.x),y(v.y),z(v.z),w(d){}
  explicit operator vec3() const { return vec3(x,y,z); }
};
inline vec2 operator+(vec2 a,vec2 b){return {a.x+b.x,a.y+b.y};}
inline vec2 operator-(vec2 a,vec2 b){return {a.x-b.x,a.y-b.y};}
inline vec2 operator*(vec2 a,float s){return {a.x*s,a.y*s};}
inline vec2 operator/(vec2 a,float s){return {a.x/s,a.y/s};}
inline vec3 operator+(vec3 a,vec3 b){return {a.x+b.x,a.y+b.y,a.z+b.z};}
inline vec3 operator-(vec3 a,vec3 b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
inline vec3 operator-(vec3 a){return {-a.x,-a.y,-a.z};}
inline vec3 operator*(vec3 a,float s){return {a.x*s,a.y*s,a.z*s};}
inline vec3 operator*(float s,vec3 a){return a*s;}
inline vec3 operator*(vec3 a,vec3 b){return {a.x*b.x,a.y*b.y,a.z*b.z};}
inline vec3 operator/(vec3 a,float s){return {a.x/s,a.y/s,a.z/s};}
inline vec4 operator+(vec4 a,vec4 b){return {a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w};}
inline vec4 operator*(vec4 a,float s){return {a.x*s,a.y*s,a.z*s,a.w*s};}
inline float length(vec2 a){return std::sqrt(a.x*a.x+a.y*a.y);}
inline float length(vec3 a){return std::sqrt(a.x*a.x+a.y*a.y+a.z*a.z);}
inline float distance(vec2 a,vec2 b){return length(a-b);}
inline float dot(vec3 a,vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
inline vec3 normalize(vec3 a){return a/length(a);}
inline vec3 cross(vec3 a,vec3 b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
inline float radians(float d){return d*0.017453292519943295f;}
inline float degrees(float r){return r*57.29577951308232f;}
inline vec2 clamp(vec2 v,vec2 lo,vec2 hi){return {std::clamp(v.x,lo.x,hi.x),std::clamp(v.y,lo.y,hi.y)};}
inline vec3 clamp(vec3 v,float lo,float hi){return {std::clamp(v.x,lo,hi),std::clamp(v.y,lo,hi),std::clamp(v.z,lo,hi)};}
inline vec3 mix(vec3 a,vec3 b,float t){return a*(1-t)+b*t;}

struct mat4 { vec4 c[4]; mat4(){} explicit mat4(float d){ c[0].x=c[1].y=c[2].z=c[3].w=d; }
  vec4& operator[](int i){return c[i];} const vec4& operator[](int i) const {return c[i];} };
struct mat3 { vec3 c[3]; mat3(){} mat3(const mat4&m){ for(int i=0;i<3;++i) c[i]=vec3(m.c[i].x,m.c[i].y,m.c[i].z);} };
inline vec4 operator*(const mat4&m,const vec4&v){ return m.c[0]*v.x+m.c[1]*v.y+m.c[2]*v.z+m.c[3]*v.w; }
inline mat4 operator*(const mat4&a,const mat4&b){ mat4 r; for(int j=0;j<4;++j) r.c[j]=a*b.c[j]; return r; }
inline mat4 transpose(const mat4&m){ mat4 r; for(int i=0;i<4;++i){ float v[4]={m.c[0].x,m.c[1].x,m.c[2].x,m.c[3].x};
  // row i of m becomes column i of r
  const float* rows[4]; (void)rows; }
  float a[4][4]; for(int c=0;c<4;++c){ a[c][0]=m.c[c].x; a[c][1]=m.c[c].y; a[c][2]=m.c[c].z; a[c][3]=m.c[c].w; }
  for(int c=0;c<4;++c) r.c[c]=vec4(a[0][c],a[1][c],a[2][c],a[3][c]); return r; }
inline mat4 inverse(const mat4&m){
  double a[4][8]; for(int r=0;r<4;++r){ double row[4]={m.c[0].x,m.c[1].x,m.c[2].x,m.c[3].x};
    if(r==1){row[0]=m.c[0].y;row[1]=m.c[1].y;row[2]=m.c[2].y;row[3]=m.c[3].y;}
    if(r==2){row[0]=m.c[0].z;row[1]=m.c[1].z;row[2]=m.c[2].z;row[3]=m.c[3].z;}
    if(r==3){row[0]=m.c[0].w;row[1]=m.c[1].w;row[2]=m.c[2].w;row[3]=m.c[3].w;}
    for(int c=0;c<4;++c){ a[r][c]=row[c]; a[r][4+c]=(r==c); } }
  for(int i=0;i<4;++i){ int p=i; for(int r=i+1;r<4;++r) if(std::fabs(a[r][i])>std::fabs(a[p][i])) p=r;
    for(int c=0;c<8;++c) std::swap(a[i][c],a[p][c]);
    double d=a[i][i]; if(d==0) return mat4(1.0f); for(int c=0;c<8;++c) a[i][c]/=d;
    for(int r=0;r<4;++r) if(r!=i){ double f=a[r][i]; for(int c=0;c<8;++c) a[r][c]-=f*a[i][c]; } }
  mat4 res; for(int c=0;c<4;++c) res.c[c]=vec4((float)a[0][4+c],(float)a[1][4+c],(float)a[2][4+c],(float)a[3][4+c]); return res; }
inline const float* value_ptr(const mat4&m){return &m.c[0].x;}
inline const float* value_ptr(const mat3&m){return &m.c[0].x;}
inline const float* value_ptr(const vec3&v){return &v.x;}
}
