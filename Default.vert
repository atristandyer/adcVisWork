layout(location=0) in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 MVPMatrix;
uniform vec4 ColorVector;

void main(void)
{
	gl_Position = MVPMatrix*in_Position;
	ex_Color = ColorVector;
}
