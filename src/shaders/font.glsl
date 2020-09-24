@module font
@ctype mat4 cute::matrix_t
@ctype vec4 cute::color_t
@ctype vec2 cute::v2

@include includes/outline.glsl

@vs vs
@glsl_options flip_vert_y
	layout (location = 0) in vec2 in_pos;
	layout (location = 1) in vec2 in_uv;

	layout (location = 0) out vec2 uv;

	layout(binding = 0) uniform vs_params {
		mat4 u_mvp;
	};

	void main()
	{
		vec4 posH = u_mvp * vec4(round(in_pos), 0, 1);
		uv = in_uv;
		gl_Position = posH;
}
@end

@fs fs
	layout (location = 0) in vec2 uv;

	out vec4 result;

	layout(binding = 0) uniform sampler2D u_image;

	layout(binding = 0) uniform fs_params {
		vec4 u_text_color;
		vec4 u_border_color;
		vec2 u_texel_size;
		float u_use_border;
		float u_use_corners;
	};

	@include_block outline

	void main()
	{
		// Border detection for pixel outlines.
		float side = sides(u_image, u_texel_size);
		float corner = corners(u_image, u_texel_size);

		// Skip corners if turned off.
		corner *= u_use_corners;

		vec4 border_color = u_border_color;
		vec4 text_color = u_text_color;
		result = mix(text_color, border_color, max(side, corner));
	}
@end

@program shd vs fs
