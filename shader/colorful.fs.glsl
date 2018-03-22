#version 330 core
out vec4 FragColor; // 输出颜色
in vec3 ourColor;   // 传入颜色

void main()
{
    FragColor = vec4(ourColor, 1.0);
}