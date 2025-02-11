#version 450 core

out vec4 FragColor;
uniform float time; 

vec3 hsv2rgb(float h, float s, float v)
{
    float c = v * s;
    float x = c * (1.0 - abs(mod(h * 6.0, 2.0) - 1.0));
    float m = v - c;
    vec3 rgb = (h < 1.0 / 6.0) ? vec3(c, x, 0) :
               (h < 2.0 / 6.0) ? vec3(x, c, 0) :
               (h < 3.0 / 6.0) ? vec3(0, c, x) :
               (h < 4.0 / 6.0) ? vec3(0, x, c) :
               (h < 5.0 / 6.0) ? vec3(x, 0, c) : vec3(c, 0, x);
    return rgb + m;
}

void main()
{
    float hue = mod(time * 0.1, 1.0);
    
    // Skip more blue (hue range ~0.5 to 0.85)
    if (hue > 0.5) {
        hue += 0.35; // Jump over the blue region
    }
    hue = mod(hue, 1.0); // Keep within valid range

    vec3 color = hsv2rgb(hue, 1.0, 1.0);
    FragColor = vec4(color, 1.0);
}
