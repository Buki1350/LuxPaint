#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;  // obrazek
uniform vec2 resolution;     // size UIObject
uniform float roundness;     // 0..1

out vec4 finalColor;

void main()
{
    vec4 texColor = texture(texture0, fragTexCoord);

    // Normalizowane współrzędne
    vec2 uv = fragTexCoord;

    // Obszar roundness w rogach
    float radius = roundness * 0.5; // 0.5 = połówka prostokąta
    vec2 dist = min(uv, 1.0 - uv); // odległość od krawędzi
    if(dist.x < radius && dist.y < radius) {
        // pixel poza okrągłym rogiem
        vec2 corner = uv;
        if(uv.x > 0.5) corner.x = 1.0 - uv.x;
        if(uv.y > 0.5) corner.y = 1.0 - uv.y;
        if(length(corner) > radius) discard;
    }

    finalColor = texColor * fragColor;
}
