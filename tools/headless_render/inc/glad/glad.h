#pragma once
#include <cstddef>
typedef unsigned int GLenum;typedef unsigned int GLuint;typedef int GLint;typedef int GLsizei;typedef float GLfloat;
typedef unsigned char GLboolean;typedef char GLchar;typedef unsigned int GLbitfield;typedef ptrdiff_t GLsizeiptr;
typedef void* (*GLADloadproc)(const char*);
#define GL_ARRAY_BUFFER 34962
#define GL_BLEND 3042
#define GL_CLAMP_TO_EDGE 33071
#define GL_COLOR_ATTACHMENT0 36064
#define GL_COLOR_BUFFER_BIT 16384
#define GL_COMPILE_STATUS 35713
#define GL_DEPTH_ATTACHMENT 36096
#define GL_DEPTH_BUFFER_BIT 256
#define GL_DEPTH_COMPONENT24 33190
#define GL_DEPTH_TEST 2929
#define GL_ELEMENT_ARRAY_BUFFER 34963
#define GL_FALSE 0
#define GL_FLOAT 5126
#define GL_FRAGMENT_SHADER 35632
#define GL_FRAMEBUFFER 36160
#define GL_FRAMEBUFFER_COMPLETE 36053
#define GL_LINEAR 9729
#define GL_LINEAR_MIPMAP_LINEAR 9987
#define GL_LINK_STATUS 35714
#define GL_MULTISAMPLE 32925
#define GL_ONE 1
#define GL_RENDERBUFFER 36161
#define GL_REPEAT 10497
#define GL_RGBA 6408
#define GL_RGBA8 32856
#define GL_SRC_ALPHA 770
#define GL_STATIC_DRAW 35044
#define GL_TEXTURE0 33984
#define GL_TEXTURE_2D 3553
#define GL_TEXTURE_MAG_FILTER 10240
#define GL_TEXTURE_MIN_FILTER 10241
#define GL_TEXTURE_WRAP_S 10242
#define GL_TEXTURE_WRAP_T 10243
#define GL_TRIANGLES 4
#define GL_TRUE 1
#define GL_UNPACK_ALIGNMENT 3317
#define GL_UNSIGNED_BYTE 5121
#define GL_UNSIGNED_INT 5125
#define GL_VERTEX_SHADER 35633
typedef GLenum (*PFN_glGetError) (); inline PFN_glGetError glGetError = nullptr;
typedef GLuint (*PFN_glCreateShader) (GLenum); inline PFN_glCreateShader glCreateShader = nullptr;
typedef void (*PFN_glShaderSource) (GLuint,GLsizei,const GLchar*const*,const GLint*); inline PFN_glShaderSource glShaderSource = nullptr;
typedef void (*PFN_glCompileShader) (GLuint); inline PFN_glCompileShader glCompileShader = nullptr;
typedef void (*PFN_glGetShaderiv) (GLuint,GLenum,GLint*); inline PFN_glGetShaderiv glGetShaderiv = nullptr;
typedef void (*PFN_glGetShaderInfoLog) (GLuint,GLsizei,GLsizei*,GLchar*); inline PFN_glGetShaderInfoLog glGetShaderInfoLog = nullptr;
typedef void (*PFN_glDeleteShader) (GLuint); inline PFN_glDeleteShader glDeleteShader = nullptr;
typedef GLuint (*PFN_glCreateProgram) (); inline PFN_glCreateProgram glCreateProgram = nullptr;
typedef void (*PFN_glAttachShader) (GLuint,GLuint); inline PFN_glAttachShader glAttachShader = nullptr;
typedef void (*PFN_glLinkProgram) (GLuint); inline PFN_glLinkProgram glLinkProgram = nullptr;
typedef void (*PFN_glGetProgramiv) (GLuint,GLenum,GLint*); inline PFN_glGetProgramiv glGetProgramiv = nullptr;
typedef void (*PFN_glGetProgramInfoLog) (GLuint,GLsizei,GLsizei*,GLchar*); inline PFN_glGetProgramInfoLog glGetProgramInfoLog = nullptr;
typedef void (*PFN_glDeleteProgram) (GLuint); inline PFN_glDeleteProgram glDeleteProgram = nullptr;
typedef void (*PFN_glUseProgram) (GLuint); inline PFN_glUseProgram glUseProgram = nullptr;
typedef GLint (*PFN_glGetUniformLocation) (GLuint,const GLchar*); inline PFN_glGetUniformLocation glGetUniformLocation = nullptr;
typedef void (*PFN_glUniform1i) (GLint,GLint); inline PFN_glUniform1i glUniform1i = nullptr;
typedef void (*PFN_glUniform1f) (GLint,GLfloat); inline PFN_glUniform1f glUniform1f = nullptr;
typedef void (*PFN_glUniform3fv) (GLint,GLsizei,const GLfloat*); inline PFN_glUniform3fv glUniform3fv = nullptr;
typedef void (*PFN_glUniformMatrix3fv) (GLint,GLsizei,GLboolean,const GLfloat*); inline PFN_glUniformMatrix3fv glUniformMatrix3fv = nullptr;
typedef void (*PFN_glUniformMatrix4fv) (GLint,GLsizei,GLboolean,const GLfloat*); inline PFN_glUniformMatrix4fv glUniformMatrix4fv = nullptr;
typedef void (*PFN_glGenVertexArrays) (GLsizei,GLuint*); inline PFN_glGenVertexArrays glGenVertexArrays = nullptr;
typedef void (*PFN_glGenBuffers) (GLsizei,GLuint*); inline PFN_glGenBuffers glGenBuffers = nullptr;
typedef void (*PFN_glBindVertexArray) (GLuint); inline PFN_glBindVertexArray glBindVertexArray = nullptr;
typedef void (*PFN_glBindBuffer) (GLenum,GLuint); inline PFN_glBindBuffer glBindBuffer = nullptr;
typedef void (*PFN_glBufferData) (GLenum,GLsizeiptr,const void*,GLenum); inline PFN_glBufferData glBufferData = nullptr;
typedef void (*PFN_glEnableVertexAttribArray) (GLuint); inline PFN_glEnableVertexAttribArray glEnableVertexAttribArray = nullptr;
typedef void (*PFN_glVertexAttribPointer) (GLuint,GLint,GLenum,GLboolean,GLsizei,const void*); inline PFN_glVertexAttribPointer glVertexAttribPointer = nullptr;
typedef void (*PFN_glDeleteVertexArrays) (GLsizei,const GLuint*); inline PFN_glDeleteVertexArrays glDeleteVertexArrays = nullptr;
typedef void (*PFN_glDeleteBuffers) (GLsizei,const GLuint*); inline PFN_glDeleteBuffers glDeleteBuffers = nullptr;
typedef void (*PFN_glDrawElements) (GLenum,GLsizei,GLenum,const void*); inline PFN_glDrawElements glDrawElements = nullptr;
typedef void (*PFN_glActiveTexture) (GLenum); inline PFN_glActiveTexture glActiveTexture = nullptr;
typedef void (*PFN_glBindTexture) (GLenum,GLuint); inline PFN_glBindTexture glBindTexture = nullptr;
typedef void (*PFN_glGenTextures) (GLsizei,GLuint*); inline PFN_glGenTextures glGenTextures = nullptr;
typedef void (*PFN_glDeleteTextures) (GLsizei,const GLuint*); inline PFN_glDeleteTextures glDeleteTextures = nullptr;
typedef void (*PFN_glPixelStorei) (GLenum,GLint); inline PFN_glPixelStorei glPixelStorei = nullptr;
typedef void (*PFN_glTexImage2D) (GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const void*); inline PFN_glTexImage2D glTexImage2D = nullptr;
typedef void (*PFN_glGenerateMipmap) (GLenum); inline PFN_glGenerateMipmap glGenerateMipmap = nullptr;
typedef void (*PFN_glTexParameteri) (GLenum,GLenum,GLint); inline PFN_glTexParameteri glTexParameteri = nullptr;
typedef void (*PFN_glGenFramebuffers) (GLsizei,GLuint*); inline PFN_glGenFramebuffers glGenFramebuffers = nullptr;
typedef void (*PFN_glBindFramebuffer) (GLenum,GLuint); inline PFN_glBindFramebuffer glBindFramebuffer = nullptr;
typedef void (*PFN_glFramebufferTexture2D) (GLenum,GLenum,GLenum,GLuint,GLint); inline PFN_glFramebufferTexture2D glFramebufferTexture2D = nullptr;
typedef void (*PFN_glGenRenderbuffers) (GLsizei,GLuint*); inline PFN_glGenRenderbuffers glGenRenderbuffers = nullptr;
typedef void (*PFN_glBindRenderbuffer) (GLenum,GLuint); inline PFN_glBindRenderbuffer glBindRenderbuffer = nullptr;
typedef void (*PFN_glRenderbufferStorage) (GLenum,GLenum,GLsizei,GLsizei); inline PFN_glRenderbufferStorage glRenderbufferStorage = nullptr;
typedef void (*PFN_glFramebufferRenderbuffer) (GLenum,GLenum,GLenum,GLuint); inline PFN_glFramebufferRenderbuffer glFramebufferRenderbuffer = nullptr;
typedef GLenum (*PFN_glCheckFramebufferStatus) (GLenum); inline PFN_glCheckFramebufferStatus glCheckFramebufferStatus = nullptr;
typedef void (*PFN_glDeleteFramebuffers) (GLsizei,const GLuint*); inline PFN_glDeleteFramebuffers glDeleteFramebuffers = nullptr;
typedef void (*PFN_glDeleteRenderbuffers) (GLsizei,const GLuint*); inline PFN_glDeleteRenderbuffers glDeleteRenderbuffers = nullptr;
typedef void (*PFN_glEnable) (GLenum); inline PFN_glEnable glEnable = nullptr;
typedef void (*PFN_glDisable) (GLenum); inline PFN_glDisable glDisable = nullptr;
typedef void (*PFN_glBlendFunc) (GLenum,GLenum); inline PFN_glBlendFunc glBlendFunc = nullptr;
typedef void (*PFN_glDepthMask) (GLboolean); inline PFN_glDepthMask glDepthMask = nullptr;
typedef void (*PFN_glViewport) (GLint,GLint,GLsizei,GLsizei); inline PFN_glViewport glViewport = nullptr;
typedef void (*PFN_glClearColor) (GLfloat,GLfloat,GLfloat,GLfloat); inline PFN_glClearColor glClearColor = nullptr;
typedef void (*PFN_glClear) (GLbitfield); inline PFN_glClear glClear = nullptr;
typedef void (*PFN_glReadPixels) (GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,void*); inline PFN_glReadPixels glReadPixels = nullptr;
inline int gladLoadGLLoader(GLADloadproc ld){ int ok=1;
  glGetError=(PFN_glGetError)ld("glGetError"); if(!glGetError){ok=0;}
  glCreateShader=(PFN_glCreateShader)ld("glCreateShader"); if(!glCreateShader){ok=0;}
  glShaderSource=(PFN_glShaderSource)ld("glShaderSource"); if(!glShaderSource){ok=0;}
  glCompileShader=(PFN_glCompileShader)ld("glCompileShader"); if(!glCompileShader){ok=0;}
  glGetShaderiv=(PFN_glGetShaderiv)ld("glGetShaderiv"); if(!glGetShaderiv){ok=0;}
  glGetShaderInfoLog=(PFN_glGetShaderInfoLog)ld("glGetShaderInfoLog"); if(!glGetShaderInfoLog){ok=0;}
  glDeleteShader=(PFN_glDeleteShader)ld("glDeleteShader"); if(!glDeleteShader){ok=0;}
  glCreateProgram=(PFN_glCreateProgram)ld("glCreateProgram"); if(!glCreateProgram){ok=0;}
  glAttachShader=(PFN_glAttachShader)ld("glAttachShader"); if(!glAttachShader){ok=0;}
  glLinkProgram=(PFN_glLinkProgram)ld("glLinkProgram"); if(!glLinkProgram){ok=0;}
  glGetProgramiv=(PFN_glGetProgramiv)ld("glGetProgramiv"); if(!glGetProgramiv){ok=0;}
  glGetProgramInfoLog=(PFN_glGetProgramInfoLog)ld("glGetProgramInfoLog"); if(!glGetProgramInfoLog){ok=0;}
  glDeleteProgram=(PFN_glDeleteProgram)ld("glDeleteProgram"); if(!glDeleteProgram){ok=0;}
  glUseProgram=(PFN_glUseProgram)ld("glUseProgram"); if(!glUseProgram){ok=0;}
  glGetUniformLocation=(PFN_glGetUniformLocation)ld("glGetUniformLocation"); if(!glGetUniformLocation){ok=0;}
  glUniform1i=(PFN_glUniform1i)ld("glUniform1i"); if(!glUniform1i){ok=0;}
  glUniform1f=(PFN_glUniform1f)ld("glUniform1f"); if(!glUniform1f){ok=0;}
  glUniform3fv=(PFN_glUniform3fv)ld("glUniform3fv"); if(!glUniform3fv){ok=0;}
  glUniformMatrix3fv=(PFN_glUniformMatrix3fv)ld("glUniformMatrix3fv"); if(!glUniformMatrix3fv){ok=0;}
  glUniformMatrix4fv=(PFN_glUniformMatrix4fv)ld("glUniformMatrix4fv"); if(!glUniformMatrix4fv){ok=0;}
  glGenVertexArrays=(PFN_glGenVertexArrays)ld("glGenVertexArrays"); if(!glGenVertexArrays){ok=0;}
  glGenBuffers=(PFN_glGenBuffers)ld("glGenBuffers"); if(!glGenBuffers){ok=0;}
  glBindVertexArray=(PFN_glBindVertexArray)ld("glBindVertexArray"); if(!glBindVertexArray){ok=0;}
  glBindBuffer=(PFN_glBindBuffer)ld("glBindBuffer"); if(!glBindBuffer){ok=0;}
  glBufferData=(PFN_glBufferData)ld("glBufferData"); if(!glBufferData){ok=0;}
  glEnableVertexAttribArray=(PFN_glEnableVertexAttribArray)ld("glEnableVertexAttribArray"); if(!glEnableVertexAttribArray){ok=0;}
  glVertexAttribPointer=(PFN_glVertexAttribPointer)ld("glVertexAttribPointer"); if(!glVertexAttribPointer){ok=0;}
  glDeleteVertexArrays=(PFN_glDeleteVertexArrays)ld("glDeleteVertexArrays"); if(!glDeleteVertexArrays){ok=0;}
  glDeleteBuffers=(PFN_glDeleteBuffers)ld("glDeleteBuffers"); if(!glDeleteBuffers){ok=0;}
  glDrawElements=(PFN_glDrawElements)ld("glDrawElements"); if(!glDrawElements){ok=0;}
  glActiveTexture=(PFN_glActiveTexture)ld("glActiveTexture"); if(!glActiveTexture){ok=0;}
  glBindTexture=(PFN_glBindTexture)ld("glBindTexture"); if(!glBindTexture){ok=0;}
  glGenTextures=(PFN_glGenTextures)ld("glGenTextures"); if(!glGenTextures){ok=0;}
  glDeleteTextures=(PFN_glDeleteTextures)ld("glDeleteTextures"); if(!glDeleteTextures){ok=0;}
  glPixelStorei=(PFN_glPixelStorei)ld("glPixelStorei"); if(!glPixelStorei){ok=0;}
  glTexImage2D=(PFN_glTexImage2D)ld("glTexImage2D"); if(!glTexImage2D){ok=0;}
  glGenerateMipmap=(PFN_glGenerateMipmap)ld("glGenerateMipmap"); if(!glGenerateMipmap){ok=0;}
  glTexParameteri=(PFN_glTexParameteri)ld("glTexParameteri"); if(!glTexParameteri){ok=0;}
  glGenFramebuffers=(PFN_glGenFramebuffers)ld("glGenFramebuffers"); if(!glGenFramebuffers){ok=0;}
  glBindFramebuffer=(PFN_glBindFramebuffer)ld("glBindFramebuffer"); if(!glBindFramebuffer){ok=0;}
  glFramebufferTexture2D=(PFN_glFramebufferTexture2D)ld("glFramebufferTexture2D"); if(!glFramebufferTexture2D){ok=0;}
  glGenRenderbuffers=(PFN_glGenRenderbuffers)ld("glGenRenderbuffers"); if(!glGenRenderbuffers){ok=0;}
  glBindRenderbuffer=(PFN_glBindRenderbuffer)ld("glBindRenderbuffer"); if(!glBindRenderbuffer){ok=0;}
  glRenderbufferStorage=(PFN_glRenderbufferStorage)ld("glRenderbufferStorage"); if(!glRenderbufferStorage){ok=0;}
  glFramebufferRenderbuffer=(PFN_glFramebufferRenderbuffer)ld("glFramebufferRenderbuffer"); if(!glFramebufferRenderbuffer){ok=0;}
  glCheckFramebufferStatus=(PFN_glCheckFramebufferStatus)ld("glCheckFramebufferStatus"); if(!glCheckFramebufferStatus){ok=0;}
  glDeleteFramebuffers=(PFN_glDeleteFramebuffers)ld("glDeleteFramebuffers"); if(!glDeleteFramebuffers){ok=0;}
  glDeleteRenderbuffers=(PFN_glDeleteRenderbuffers)ld("glDeleteRenderbuffers"); if(!glDeleteRenderbuffers){ok=0;}
  glEnable=(PFN_glEnable)ld("glEnable"); if(!glEnable){ok=0;}
  glDisable=(PFN_glDisable)ld("glDisable"); if(!glDisable){ok=0;}
  glBlendFunc=(PFN_glBlendFunc)ld("glBlendFunc"); if(!glBlendFunc){ok=0;}
  glDepthMask=(PFN_glDepthMask)ld("glDepthMask"); if(!glDepthMask){ok=0;}
  glViewport=(PFN_glViewport)ld("glViewport"); if(!glViewport){ok=0;}
  glClearColor=(PFN_glClearColor)ld("glClearColor"); if(!glClearColor){ok=0;}
  glClear=(PFN_glClear)ld("glClear"); if(!glClear){ok=0;}
  glReadPixels=(PFN_glReadPixels)ld("glReadPixels"); if(!glReadPixels){ok=0;}
  return ok; }
