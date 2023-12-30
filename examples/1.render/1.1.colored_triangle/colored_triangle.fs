#version 330 core

out vec4 FragColor;

in vec3 user_color;

void main()
{
    FragColor = vec4(user_color, 1.0f);
}