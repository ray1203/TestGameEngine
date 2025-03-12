cbuffer constants : register(b0)
{
}

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 color : COLOR; // Color to pass to the pixel shader
};

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
