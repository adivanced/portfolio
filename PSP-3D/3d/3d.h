#ifndef ___3D_H___
#define ___3D_H___

#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <list>


namespace d3{
	struct vec_3d{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;

		
	};

	struct triangle{
		vec_3d points[3];
		
		uint32_t color;
	};

	//always define triangles clockwise

	struct mesh{
		std::vector<triangle> triangles;

		bool load_obj_file(const char[]);
	};


	struct proj_matrix{
		float m[4][4] = {};
	};

	inline float q_rsqrt(float) noexcept;

	inline float vector_dot_product(vec_3d&, vec_3d&) noexcept;

	inline float vector_length(vec_3d&) noexcept;

	inline vec_3d vector_sub(vec_3d&, vec_3d&) noexcept;

	inline vec_3d vector_normalise(vec_3d&) noexcept;

	inline vec_3d vector_normalise_l(vec_3d) noexcept;

	inline vec_3d vector_add(vec_3d&, vec_3d&) noexcept;

	inline void vector_add_self(vec_3d&, vec_3d&) noexcept;

	inline vec_3d vector_mul(vec_3d&, float) noexcept;

	inline vec_3d vector_div(vec_3d&, float) noexcept;

	inline vec_3d vector_cross_product(vec_3d&, vec_3d&) noexcept;

	inline vec_3d vector_cross_product_l(vec_3d, vec_3d) noexcept;	

	vec_3d vector_intersect_plane(vec_3d&, vec_3d&, vec_3d&, vec_3d&) noexcept;

	int triangle_clips_against_plane(vec_3d , vec_3d , triangle& , triangle& , triangle& ) noexcept;

	void triangle_clips_against_plane_w_check(vec_3d, vec_3d, triangle& , triangle& , triangle& , bool& , bool& , bool& ) noexcept;

	vec_3d multiply_matrix_vect(proj_matrix&, vec_3d&) noexcept;

	inline proj_matrix matrix_make_identity() noexcept;

	proj_matrix matrix_make_rotation_x(float) noexcept;

	proj_matrix matrix_make_rotation_y(float) noexcept;

	proj_matrix matrix_make_rotation_z(float) noexcept;

	inline proj_matrix matrix_make_translation(float, float, float) noexcept;

	inline proj_matrix matrix_projection(float, float, float, float) noexcept;

	inline proj_matrix multiply_matrix_matr(proj_matrix&, proj_matrix&) noexcept;

	proj_matrix matrix_point_at(vec_3d&, vec_3d&, vec_3d&) noexcept;

	proj_matrix matrix_quick_inverse(proj_matrix&) noexcept;

	void multiply_matrix(vec_3d&, vec_3d&, proj_matrix&) noexcept;

	void draw_triangle(float, float, float, float, float, float, uint32_t) noexcept;

	void fill_triangle(int, int, int, int, uint32_t) noexcept;

	inline void draw_triangle_depth(float, float, float, float, float, float, uint32_t, float) noexcept;

	void fill_triangle_depth(int, int, int, int, uint32_t, float) noexcept;

	inline uint32_t get_shaded_color(float, uint32_t) noexcept;

	void draw_mesh(mesh&, uint32_t, proj_matrix&, proj_matrix&, proj_matrix&, proj_matrix&) noexcept;

};

long long int current_timestamp() noexcept;


#endif