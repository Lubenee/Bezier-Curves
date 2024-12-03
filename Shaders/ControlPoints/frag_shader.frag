#version 450 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.9, 0.9, 0.9, 1.0); // white-ish
}

//TODOS For ImGUI:
// Control point size
// Control point color, glow?
// Animate their positions over time, translation&rotation ?