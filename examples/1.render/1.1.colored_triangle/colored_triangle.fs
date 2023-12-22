#version 330 core

out vec4 FragColor;

uniform float g_time;

void main()
{
    vec4 result_color = vec4(0.0f, sin(g_time / 1000.0f) / 2.0 + 0.5, 0.0f, 1.0f);

    FragColor = result_color;
}