#version 330 core

layout(location = 0) out vec4 OutHitPos;
layout(location = 1) out vec4 OutHitNormal;
layout(location = 2) out vec4 OutRayDir;
layout(location = 3) out vec4 OutColor; // with material ID attached

uniform sampler2D SceneData;
uniform sampler2D MatData;

uniform sampler2D TexArr[16];