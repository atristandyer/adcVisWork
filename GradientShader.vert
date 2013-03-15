layout(location=0) in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 MVPMatrix;
uniform vec4 LowColor;
uniform vec4 HighColor;
uniform vec2 HeightRange;

void main(void)
{
	if (in_Position.z < 0.0 && in_Position.z > -0.0)
	{
		ex_Color.r = 0.0;
		ex_Color.g = 0.0;
		ex_Color.b = 0.0;
		ex_Color.a = 1.0;
	} else {
		float range = HeightRange[1] - HeightRange[0];
		if (range != 0)
		{
			float percentage = ((in_Position.z - HeightRange[0])/range);
			ex_Color.r = LowColor.r + percentage*(HighColor.r - LowColor.r);
			ex_Color.g = LowColor.g + percentage*(HighColor.g - LowColor.g);
			ex_Color.b = LowColor.b + percentage*(HighColor.b - LowColor.b);
			ex_Color.a = LowColor.a + percentage*(HighColor.a - LowColor.a);
		} else {
			ex_Color = LowColor;
		}
	}

	gl_Position = MVPMatrix*in_Position;
}



//// Test four color gradient
//void main(void)
//{
//	float binSize = (HeightRange[1]-HeightRange[0])/3.0;
//	if (binSize != 0)
//	{
//		if (in_Position.z >= HeightRange[0] && in_Position.z < HeightRange[0]+binSize)
//		{
//			float percentage = ((in_Position.z-HeightRange[0])/binSize);
//			ex_Color.r = 0.0;
//			ex_Color.g = percentage;
//			ex_Color.b = 1.0-percentage;
//			ex_Color.a = LowColor.a;
//		}
//		else if (in_Position.z >= HeightRange[0]+binSize && in_Position.z < HeightRange[0]+2*binSize)
//		{
//			float percentage = ((in_Position.z-HeightRange[0]-binSize)/binSize);
//			ex_Color.r = percentage;
//			ex_Color.g = 1.0;
//			ex_Color.b = 0.0;
//			ex_Color.a = LowColor.a;
//		}
//		else
//		{
//			float percentage = ((in_Position.z-HeightRange[0]-2*binSize)/binSize);
//			ex_Color.r = 1.0;
//			ex_Color.g = 1-percentage;
//			ex_Color.b = 0.0;
//			ex_Color.a = LowColor.a;
//		}
//	} else {
//		ex_Color = LowColor;
//	}

//	gl_Position = MVPMatrix*in_Position;
//}
