#version 330 core

out vec4 FragColor;

in vec3 user_color;
in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = texture(texture2, tex_coord);//mix(texture(texture1, tex_coord), texture(texture2, tex_coord), 0.2);// * vec4(user_color, 1.0);
}