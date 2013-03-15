layout(location=0) in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 MVPMatrix;
uniform vec4 LowColor;
uniform vec4 HighColor;
uniform vec4 WDColor;
uniform vec4 HeightRange;

void main(void)
{
	if (in_Position.z == 99999)
	{
		ex_Color = WDColor;
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
