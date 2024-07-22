#version 330 core

// Classes
struct Vertex {
    vec3 pos;
    vec3 normal;
    vec2 uv;
};
struct Ray {
    vec3 origin;
    vec3 dir;
    highp vec3 color;
    float tMax;
};
struct Hit {
    bool isHit;
    struct Vertex vertex;
    float matIdx;
};
struct Camera {
    vec3 pos;
    mat4 viewProj;
    float scrW;
    float scrH;
};

// Constants
const float PI = 3.1415926;
const float tMin = 0.001; // for RayHit smallest Distance
const float FLT_MAX = 99999999999999999999999999999999999999.0;
const int ITR_GROUP = 0;
const int ITR_TRANSFORM = 1;
const int ITR_MODEL = 2;
const int ITR_MESH = 3;
const int ITR_SUBMESH = 4;
const int ITR_TRIANGLE = 5;

const int MAT_BLINNPHONG = 0;
const int MAT_PBR = 1;

// GlobVariables
int rdCnt = 0;

// Functions Definitions
float RandXY(float x, float y); // [0.0, 1.0)
float Rand();                   // [0.0, 1.0)
vec2 RandInSquare();
vec2 RandInCircle();
vec3 RandInSphere();

void Camera_GenRay(out struct Ray ray);

// GLSL IN OUT
out vec4 FragColor;
in vec2 TexCoords;

// GLSL UNIFORM
uniform struct Camera u_camera;
uniform float u_rdSeed[4];
uniform sampler2D u_sceneData;
uniform sampler2D u_matData;

void main()
{
    FragColor = vec4(TexCoords, 0.0, 1.0);
}

float RandXY(float x, float y) {
    return fract(cos(dot(vec2(x, y), vec2(12.9898, 4.1414))) * 43758.5453);
}

float Rand() {
    float a = RandXY(TexCoords.x, u_rdSeed[0]);
    float b = RandXY(u_rdSeed[1], TexCoords.y);
    float c = RandXY(rdCnt++, u_rdSeed[2]);
    float d = RandXY(u_rdSeed[3], a);
    float e = RandXY(b, c);
    float f = RandXY(d, e);

    return f;
}

vec2 RandInSquare() {
    return vec2(Rand(), Rand());
}

vec2 RandInCircle() {
    vec2 rst;
    do {
        rst = vec2(Rand(), Rand()) * 2.0f - 1.0f;
    } while (dot(rst, rst) >= 1.0);
    return rst;
}

vec3 RandInSphere() {
    vec3 rst;
    do {
        rst = vec3(Rand(), Rand(), Rand()) * 2.0f - 1.0f;
    } while (dot(rst, rst) >= 1.0);
    return rst;
}

void Camera_GenRay(out struct Ray ray) {
    vec2 st = TexCoords + RandInSquare() / vec2(u_camera.scrW, u_camera.scrH);

    // convert SS to NDC
    vec4 ndcPos = vec4(2.0 * TexCoords - 1.0, -1.0, 1.0);

    // convert NDC to clip space
    vec4 clipSpacePos = ndcPos;
    clipSpacePos.z = -1.0;
    clipSpacePos.w = 1.0;

    // convert clip space to world space
    vec4 worldSpacePos = inverse(u_camera.viewProj) * clipSpacePos;
    
    // calculate ray direction
    ray.origin = worldSpacePos.xyz / worldSpacePos.w;
    ray.dir = normalize(ray.origin - u_camera.pos);
    ray.color = vec3(1);
    ray.tMax = FLT_MAX;
}

vec4 GetPack(sampler2D data, int idx) {
    idx /= 4;
    ivec2 texSize = textureSize(data, 0);

    float offsetu = idx / texSize.y + 0.5;
    float offsetv = idx % texSize.y + 0.5;

    vec2 texCoords = vec2(offsetu / float(texSize.x), offsetv / float(texSize.y));
    return texture(data, texCoords);
}

int At(sampler2D data, int idx) {
    int offsetColor = idx % 4;
    idx /= 4;
    ivec2 texSize = textureSize(data, 0);

    float offsetu = idx / texSize.y + 0.5;
    float offsetv = idx % texSize.y + 0.5;

    vec2 texCoords = vec2(offsetu / float(texSize.x), offsetv / float(texSize.y));
    vec4 value = texture(data, texCoords);
    if (offsetColor == 0) return int(value.x);
    if (offsetColor == 1) return int(value.y);
    if (offsetColor == 2) return int(value.z);
    if (offsetColor == 3) return int(value.w);
}


void Ray_Transform(inout struct Ray ray, mat4 transform) {
    ray.dir = mat3(transform) * ray.dir;
    vec4 originQ = transform * vec4(ray.origin, 1.0f);
    ray.origin = originQ.xyz / originQ.w;
}

void Vertex_Load(int idx, out struct Vertex vert) {
    vec4 pos_u = GetPack(u_sceneData, idx);
    vec4 normal_v = GetPack(u_sceneData, idx+4);

    vert.pos = pos_u.xyz;
    vert.normal = normal_v.xyz;
    vert.uv = vec2(pos_u[3], normal_v[3]);
}

void Vertex_Interpolate(vec3 abg, struct Vertex A, struct Vertex B, struct Vertex C, out struct Vertex vert) {
    vert.uv[0] = dot(abg, vec3(A.uv[0], B.uv[0], C.uv[0]));
    vert.uv[1] = dot(abg, vec3(A.uv[1], B.uv[1], C.uv[1]));
    vert.pos = abg[0] * A.pos + abg[1] * B.pos + abg[2] * C.pos;
    vert.normal = abg[0] * A.normal + abg[1] * B.normal + abg[2] * C.normal;
}

void Vertex_Transform(inout struct Vertex vert, mat4 transform) {
    vec4 posQ = transform * vec4(vert.pos, 1.0);
    vert.pos = posQ.xyz / posQ.w;
    vert.normal = normalize(transpose(inverse(mat3(transform))) * vert.normal);
}

vec4 Intersect_RayTri(vec3 e, vec3 d, vec3 a, vec3 b, vec3 c) {
    mat3 equation_A = mat3(vec3(a - b), vec3(a - c), d);

    if (abs(determinant(equation_A)) < 0.00001)
        return vec4(0);

    vec3 equation_b = a - e;
    vec3 equation_X = inverse(equation_A) * equation_b;
    float alpha = 1 - equation_X[0] - equation_X[1];
    return vec4(alpha, equation_X);
}

void RayHit_Triangle(int idx, inout struct Ray ray, inout struct Hit hit) {
    struct Vertex A, B, C;
    Vertex_Load(idx, A);
    Vertex_Load(idx + 8, B);
    Vertex_Load(idx + 16, C);

    vec4 abgt = Intersect_RayTri(ray.origin, ray.dir, A.pos, B.pos, C.pos);
    if (abgt == vec4(0) ||
        any(lessThan(abgt, vec4(0, 0, 0, tMin))) ||
        any(greaterThan(abgt, vec4(1, 1, 1, ray.tMax)))
        )
        return;

    hit.isHit = true;
    Vertex_Interpolate(abgt.xyz, A, B, C, hit.vertex);
    ray.tMax = abgt[3];
}

void RayHit(in struct Ray ray, out struct Hit hit) {
    int TreePtr = 0;
    int curMat = 0;
    do {
        if (TreePtr < 0) {
            // return to upper level's next child
            TreePtr = At(u_sceneData, -TreePtr);
            continue;
        }

        vec4 node = GetPack(u_sceneData, TreePtr);
        int Itr = int(node.x);
        if (Itr == ITR_GROUP) {
            TreePtr = At(u_sceneData, TreePtr+4);
        }
        else if (Itr == ITR_TRANSFORM) {
			TreePtr = At(u_sceneData, TreePtr+4);
            mat4 modelMat = mat4(
				GetPack(u_sceneData, TreePtr),
				GetPack(u_sceneData, TreePtr+4),
				GetPack(u_sceneData, TreePtr+8),
				GetPack(u_sceneData, TreePtr+12)
			);
            Ray_Transform(ray, modelMat);
            if(hit.isHit)
                Vertex_Transform(hit.vertex, modelMat);
        }
        else if (Itr == ITR_MODEL || Itr == ITR_MESH) {
            TreePtr = int(node.z);
        }
        else if (Itr == ITR_SUBMESH) {
            curMat = int(node.z);
            TreePtr = int(node.w);
        }
        else if (Itr == ITR_TRIANGLE) {
            RayHit_Triangle(TreePtr+4, ray, hit);
            TreePtr = int(node.z);
		}
        else {
            // for Crash Propose
            1.0 / 0.0;
        }

    } while (TreePtr != 0);
}