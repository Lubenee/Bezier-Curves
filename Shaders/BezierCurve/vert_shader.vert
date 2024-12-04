#version 450 core

layout(location = 0) in vec2 inPos;

uniform float time; // ensures changing values
uniform float waveAmplitude; // scales the sine wave's amplitude (height)
uniform float waveFrequency; // how fast the sine wave oscillates
uniform float pulseSpeed; // how fast to shrink and grow

void main()
{
    float pulse = 1.0 + 0.1 * sin(time * pulseSpeed);
    float waveOffset = waveAmplitude * sin(inPos.x * waveFrequency + time);
    gl_Position = vec4(inPos.x * pulse, (inPos.y + waveOffset) * pulse, 0.0, 1.0);
}

