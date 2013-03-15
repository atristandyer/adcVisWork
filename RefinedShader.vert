layout(location=0) in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 MVPMatrix;
uniform vec4 BottomColor;
uniform vec4 TopColor;
uniform vec4 NoErrorColor;
uniform vec2 NoErrorRange;
uniform vec2 ErrorRange;

void main(void)
{
	if (in_Position[2] == -99999 ^^ in_Position[3] == -99999)
	{
		if (in_Position[2] == -99999)
		{
			gl_Position = MVPMatrix*vec4(in_Position.x, in_Position.y, in_Position[3], 1.0);
			ex_Color = vec4(0.0, 0.0, 0.0, 1.0);
		} else {
			gl_Position = MVPMatrix*vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
			ex_Color = vec4(0.0, 0.0, 0.0, 1.0);
		}
		return;
	}
	else if (in_Position[2] == -99999 && in_Position[3] == -99999)
	{
		gl_Position = MVPMatrix*vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
		ex_Color = vec4(1.0, 1.0, 1.0, 0.0);
		return;
	}
	float error = in_Position[3] - in_Position[2];
	if (error < NoErrorRange[0] || error > NoErrorRange[1])
	{
		float range = ErrorRange[1] - ErrorRange[0];
		if (range != 0)
		{
			float percentage = ((error-ErrorRange[0])/range);
			ex_Color.r = BottomColor.r + percentage*(TopColor.r - BottomColor.r);
			ex_Color.g = BottomColor.g + percentage*(TopColor.g - BottomColor.g);
			ex_Color.b = BottomColor.b + percentage*(TopColor.b - BottomColor.b);
			ex_Color.a = BottomColor.a + percentage*(TopColor.a - BottomColor.a);
		} else {
			ex_Color = BottomColor;
		}
	} else {
		ex_Color = NoErrorColor;
	}
	gl_Position = MVPMatrix*vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}
