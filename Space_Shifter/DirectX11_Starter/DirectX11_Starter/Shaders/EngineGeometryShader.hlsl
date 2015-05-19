
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float3 camPos;
};

struct VStoGS
{
	int type            : TEXCOORD0;
	float3 position	    : POSITION;
	float4 color        : COLOR;
	float size          : TEXCOORD1;
};


struct GStoPS
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
	float2 uv : UV;
};


[maxvertexcount(4)]
void main(point VStoGS input[1], inout TriangleStream<GStoPS> outStream)
{
	//compute world space directions
	float3 look = normalize(camPos.xyz - input[0].position);
	float3 right = normalize(cross(float3(0, 1, 0), look));
	float3 up = cross(look, right);

	//calc size, half width half hieight
	float2 size = float2(1.0f, 1.0f);
	float halfW = 0.5f * size.x;
	float halfH = 0.5f * size.y;

	//create array to hold the 4 new vertex positions
	float4 v[4];

	v[0] = float4(input[0].position + halfW*right - halfH*up, 1);
	v[1] = float4(input[0].position + halfW*right + halfH*up, 1);
	v[2] = float4(input[0].position - halfW*right - halfH*up, 1);
	v[3] = float4(input[0].position - halfW*right + halfH*up, 1);


	// Define array of texture coordinates to match the four corners of the quad,
	// allowing us to loop over the four uv coords easily.
	float2 quadUVs[4] =
	{
		float2(1, 1),
		float2(1, 0),
		float2(0, 1),
		float2(0, 0)
	};

	// Calculate world view proj matrix
	matrix wvp = mul(mul(world, view), projection);

	// Finalize the GS output by appending 4 verts worth of data
	GStoPS vert; //holds a single vertex

	[unroll]
	for (int i = 0; i < 4; i++)
	{
		//alrleady have the world position, need to multiply by the view and projection matricies
		
		//vert.position = mul(v[i], mul(view, projection));
		// Create a single vertex and add to the stream
		vert.position = mul(float4(input[0].position, 1.0f), wvp);

		// Depth stuff
		float depthChange = vert.position.z / vert.position.w;

		// Adjust based on depth
		vert.position.xy += v[i].xy * depthChange * input[0].size;

		vert.uv = quadUVs[i];
		vert.color = input[0].color;
		outStream.Append(vert); //apppend this vertex!
	}
}
	/*
	// Don't draw root particle itself
	if (input[0].type == 0)
		return;

	GStoPS output;

	// Offsets for smaller triangles
	float4 offsets[3];
	offsets[0] = float4(    0, +0.1f, -0.1f, 0);
	offsets[1] = float4(+0.1f, -0.1f, -0.1f, 0);
	offsets[2] = float4(-0.1f, -0.1f, -0.1f, 0);

	// Calculate world view proj matrix
	matrix wvp = mul(mul(world, view), projection);
	
	// For each vert, spit out a small triangle
	[unroll]
	for (int o = 0; o < 3; o++)
	{
		// Create a single vertex and add to the stream
		output.position = mul(float4(input[0].position, 1.0f), wvp);
		
		// Depth stuff
		float depthChange = output.position.z / output.position.w;

		// Adjust based on depth
		output.position.xy += offsets[o].xy * depthChange * input[0].size;
		output.color = input[0].color;

		// Done
		outStream.Append(output);
	}

	}*/