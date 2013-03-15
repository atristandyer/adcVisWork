in vec4 ex_Color;
out vec4 out_Color;

void main(void)
{
	if (ex_Color.a == 0.0)
		discard;
	out_Color = ex_Color;
}
