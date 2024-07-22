#version 330 core

struct Ray {
    vec3 origin;
    vec3 dir;
    highp vec3 color;
    float tMax;
    float time;
};

struct Camera {
    vec3 pos;
    vec3 BL_Corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 right;
    vec3 up;
    vec3 front;
    float lenR;
    float t0;
    float t1;
};

const float PI = 3.1415926;
const float tMin = 0.001;
const float FLT_MAX = 99999999999999999999999999999999999999.0;

void Camera_GenRay(in struct camera, in vec2 texCoords, out struct Ray ray) {
    vec2 st = texCoords + RandInSquare() / textureSize(origin_curRayNum, 0);
    vec2 rd = camera.lenR * RandInCircle();

    ray.origin = camera.pos + rd.x * camera.right + rd.y * camera.up;
    ray.dir = camera.BL_Corner + st.s * camera.horizontal + st.t * camera.vertical - ray.origin;
    ray.color = vec3(1);
    ray.tMax = FLT_MAX;
    ray.time = mix(camera.t0, camera.t1, Rand());
}