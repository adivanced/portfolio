#include "3d.h"
//#include "../common/graphics.cpp"
#include "../common/graphics.h"

extern inline void bresenham_depth(int, int, int, int, uint32_t, float) noexcept;
extern inline void pixel_depth(int, int, int, int, uint32_t, float) noexcept;
extern inline void clear_z_buffer() noexcept;


extern d3::vec_3d camera;

namespace d3{ 
	bool mesh::load_obj_file(const char filename[]){
		FILE* file;
		//pspDebugScreenPrintf("%s", "POINT 1! \n");
		file = fopen(filename, "r");

		char buf[128];

		std::vector<vec_3d> pool;
		//pspDebugScreenPrintf("%s", "POINT 2! \n");
		while(fgets(buf, 128, file)){
	//		pspDebugScreenPrintf("%s", "POINT 3! \n");
			if(buf[0] == 'v' or buf[0] == 'f'){
				//cout << buf << endl;
				std::stringstream ss;
				for(int i = 1; buf[i] != '\0'; i++){
					ss << buf[i];
				}
				//pspDebugScreenPrintf("%s", "POINT 4! \n");
				if(buf[0] == 'v'){
					vec_3d v;
					ss >> v.x >> v.y >> v.z;
					pool.push_back(v);
					//pspDebugScreenPrintf("%s", "POINT 5(1)! \n");
				}else if(buf[0] == 'f'){
					int tri[3];
					ss >> tri[0] >> tri[1] >> tri[2];
					triangles.push_back({pool[tri[0] - 1], pool[tri[1] - 1], pool[tri[2] - 1] });
					//pspDebugScreenPrintf("%s", "POINT 5(2)! \n");
				}
				//cout << endl;
			}
			//pspDebugScreenPrintf("%s", "POINT 6! \n");
		}
		fclose(file);
		//pspDebugScreenPrintf("%s", "POINT 7! \n");
		return true;
	}

	inline float q_rsqrt(float number) noexcept{
		float  y;
		long i;
		
		y = number;
		i = *((long*) &y);
		i = 0x5f3759df - (i >> 1);
		y = *((float*) &i);
		return y *(1.5f - (number * 0.5f * y * y));
	}

	inline bool operator==(vec_3d a, vec_3d b){
		return ((a.x == b.x) * (a.y == b.y) * (a.z == b.z));
	}
	inline bool operator!=(vec_3d a, vec_3d b){
		return !(a == b);
	}

	inline float vector_dot_product(vec_3d &v1, vec_3d &v2) noexcept{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	inline float vector_length(vec_3d& v) noexcept{
		return sqrtf(vector_dot_product(v, v));
		//this actually is not length but 1/length
	}	


	inline vec_3d vector_sub(vec_3d& v1, vec_3d& v2) noexcept{
		return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	}


	inline vec_3d vector_normalise(vec_3d& v) noexcept{
		//float l = vector_length(v);
		//return {v.x / l, v.y / l, v.z / l};

		float l = q_rsqrt(vector_dot_product(v, v));
		return {v.x * l, v.y * l, v.z * l};
	}

	inline vec_3d vector_normalise_l(vec_3d v) noexcept{
		//float l = vector_length(v);
		//return {v.x / l, v.y / l, v.z / l};

		float l = q_rsqrt(vector_dot_product(v, v));
		return {v.x * l, v.y * l, v.z * l};
	}

	inline vec_3d vector_add(vec_3d& v1, vec_3d& v2) noexcept{
		return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	}

	inline void vector_add_self(vec_3d& v1, vec_3d& v2) noexcept{
		//return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
	}

	inline vec_3d vector_mul(vec_3d& v1, float k) noexcept{
		return {v1.x * k, v1.y * k, v1.z * k};
	}

	inline vec_3d vector_div(vec_3d& v1, float k) noexcept{
		return {v1.x / k, v1.y / k, v1.z / k};
	}

	inline vec_3d vector_cross_product(vec_3d& v1, vec_3d& v2) noexcept{
		vec_3d v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	inline vec_3d vector_cross_product_l(vec_3d v1, vec_3d v2) noexcept{
		vec_3d v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;

		//return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x}; SADLY, THE SAME
	}

	vec_3d vector_intersect_plane(vec_3d& plane_p, vec_3d& plane_n, vec_3d& lineStart, vec_3d& lineEnd)
	{
		plane_n = vector_normalise(plane_n);
		float plane_d = -vector_dot_product(plane_n, plane_p);
		float ad = vector_dot_product(lineStart, plane_n);
		float bd = vector_dot_product(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		vec_3d lineStartToEnd = vector_sub(lineEnd, lineStart);
		vec_3d lineToIntersect = vector_mul(lineStartToEnd, (-plane_d - ad) / (bd - ad));
		return vector_add(lineStart, lineToIntersect);
	}

	int triangle_clips_against_plane(vec_3d plane_p, vec_3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2){
		// Make sure plane normal is indeed normal
		plane_n = vector_normalise(plane_n);

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](vec_3d &p)
		{
			//vec_3d n = vector_normalise(p);
			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - vector_dot_product(plane_n, plane_p));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		vec_3d* inside_points[3];  int nInsidePointCount = 0;
		vec_3d* outside_points[3]; int nOutsidePointCount = 0;

		// Get signed distance of each point in triangle to plane
		float d0 = dist(in_tri.points[0]);
		float d1 = dist(in_tri.points[1]);
		float d2 = dist(in_tri.points[2]);

		if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[0]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.points[0]; }
		if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[1]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.points[1]; }
		if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[2]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.points[2]; }

		// Now classify triangle points, and break the input triangle into 
		// smaller output triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist

			return 0; // No returned triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			out_tri1 = in_tri;

			return 1; // Just the one returned original triangle is valid
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			out_tri1.color =  in_tri.color;
			

			// The inside point is valid, so keep that...
			out_tri1.points[0] = *inside_points[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			out_tri1.points[1] = vector_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
			out_tri1.points[2] = vector_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

			return 1; // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			out_tri1.color =  in_tri.color;

			out_tri2.color =  in_tri.color;

			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1.points[0] = *inside_points[0];
			out_tri1.points[1] = *inside_points[1];
			out_tri1.points[2] = vector_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri2.points[0] = *inside_points[1];
			out_tri2.points[1] = out_tri1.points[2];
			out_tri2.points[2] = vector_intersect_plane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

			return 2; // Return two newly formed triangles which form a quad
		}
		return 0;
	}

	vec_3d multiply_matrix_vect(proj_matrix& m, vec_3d& i) noexcept{
		vec_3d v;
		v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
		v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
		v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
		v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
		return v;
	}

	inline proj_matrix matrix_make_identity() noexcept{
		proj_matrix matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	proj_matrix matrix_make_rotation_x(float fangle_rad) noexcept{
		proj_matrix matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = cosf(fangle_rad);
		matrix.m[1][2] = sinf(fangle_rad);
		matrix.m[2][1] = -sinf(fangle_rad);
		matrix.m[2][2] = cosf(fangle_rad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	proj_matrix matrix_make_rotation_y(float fangle_rad) noexcept{
		proj_matrix matrix;
		matrix.m[0][0] = cosf(fangle_rad);
		matrix.m[0][2] = sinf(fangle_rad);
		matrix.m[2][0] = -sinf(fangle_rad);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(fangle_rad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	proj_matrix matrix_make_rotation_z(float fangle_rad) noexcept{
		proj_matrix matrix;
		matrix.m[0][0] = cosf(fangle_rad);
		matrix.m[0][1] = sinf(fangle_rad);
		matrix.m[1][0] = -sinf(fangle_rad);
		matrix.m[1][1] = cosf(fangle_rad);
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	inline proj_matrix matrix_make_translation(float x, float y, float z) noexcept{
		proj_matrix matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;
		return matrix;
	}

	inline proj_matrix matrix_projection(float ffov_degrees, float faspect_ratio, float fnear, float ffar) noexcept{
		float ffov_rad = 1.0f / tanf(ffov_degrees * 0.5f / 180.0f * 3.14159f);
		proj_matrix matrix;
		matrix.m[0][0] = faspect_ratio * ffov_rad;
		matrix.m[1][1] = ffov_rad;
		matrix.m[2][2] = ffar / (ffar - fnear);
		matrix.m[3][2] = (-ffar * fnear) / (ffar - fnear);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;
		return matrix;
	}

	inline proj_matrix multiply_matrix_matr(proj_matrix& m1, proj_matrix& m2) noexcept{
		proj_matrix matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;
	}

	proj_matrix matrix_point_at(vec_3d& pos, vec_3d& target, vec_3d& up) noexcept{
		vec_3d new_forward = vector_sub(target, pos);
		new_forward = vector_normalise(new_forward);

		vec_3d a = vector_mul(new_forward, vector_dot_product(up, new_forward));
		vec_3d new_up = vector_sub(up, a);
		new_up = vector_normalise(new_up);

		vec_3d new_right = vector_cross_product(new_up, new_forward);

		proj_matrix matrix;
		matrix.m[0][0] = new_right.x;	matrix.m[0][1] = new_right.y;	matrix.m[0][2] = new_right.z;	matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = new_up.x;		matrix.m[1][1] = new_up.y;		matrix.m[1][2] = new_up.z;		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = new_forward.x;	matrix.m[2][1] = new_forward.y;	matrix.m[2][2] = new_forward.z;	matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
		return matrix;

	}

	proj_matrix matrix_quick_inverse(proj_matrix &m) noexcept// Only for Rotation/Translation Matrices
	{
		proj_matrix matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	void fill_triangle_depth(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color, float depth) noexcept
	{
		auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		
		
		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
		if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
		if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx>t1x){ minx = t1x;}
			if (minx>t2x) {minx = t2x;}
			if (maxx<t1x) {maxx = t1x;}
			if (maxx<t2x) {maxx = t2x;}
			disp::bresenham_depth(minx, y, maxx, y, color, depth);    // disp::pixel line from min to max points found on the y
			//disp::pixel_depth(10, 10, 10, 0.0f);	 // Now increase y
			if (!changed1) {t1x += signx1;}
			t1x += t1xp;
			if (!changed2) {t2x += signx2;}
			t2x += t2xp;
			y += 1;
			if (y == y2) {break;}

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i<dx1) i++;
			} 
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) {t2xp = signx2;}
					else{goto next4;}
				}
				if (changed2){break;}
				else {t2x += signx2;}
			}
		next4:

			if (minx>t1x){ minx = t1x;} if (minx>t2x) {minx = t2x;}
			if (maxx<t1x) {maxx = t1x;} if (maxx<t2x) {maxx = t2x;}
			disp::bresenham_depth(minx, y, maxx, y, color, depth);   										
			if (!changed1) {t1x += signx1;}
			t1x += t1xp;
			if (!changed2) {t2x += signx2;}
			t2x += t2xp;
			y += 1;
			if (y>y3) {return;}
		}
	}

	//COLORS - ABGR
	inline uint32_t get_shaded_color(float lit, uint32_t base_color) noexcept{
		return base_color + ( ((uint32_t)(0x20 * lit) << 16) + ((uint32_t)(0x20 * lit) << 8) + ((uint32_t)(0x20 * lit)));
		//return 0x0000ff00;
	}	//0x3c


	void draw_mesh(mesh& msh, uint32_t color, proj_matrix& matProj, proj_matrix& matr_world, proj_matrix& matr_view) noexcept{
		vec_3d line1, line2;
		for(unsigned int i = 0; i < msh.triangles.size(); i++){
			triangle triang_projected;
			//triangle triang_transformed;
			//triangle triang_viewed;


			triang_projected.points[0] = multiply_matrix_vect(matr_world, msh.triangles[i].points[0]);
			triang_projected.points[1] = multiply_matrix_vect(matr_world, msh.triangles[i].points[1]);
			triang_projected.points[2] = multiply_matrix_vect(matr_world, msh.triangles[i].points[2]);
			

			//line1 = vector_sub(triang_transformed.points[1], triang_transformed.points[0]);
			//line2 = vector_sub(triang_transformed.points[2], triang_transformed.points[0]);
			//vec_3d normal = vector_cross_product(line1, line2);
			//normal = vector_normalise(normal);
			vec_3d normal = vector_normalise_l(vector_cross_product_l(vector_sub(triang_projected.points[1], triang_projected.points[0]), vector_sub(triang_projected.points[2], triang_projected.points[0])));


			vec_3d camera_ray = vector_sub(triang_projected.points[0], camera);

			if(vector_dot_product(normal, camera_ray) < 0.0f){

				vec_3d light_direction = {0.0f, 0.0f, -1.0f};
				light_direction = vector_normalise(light_direction);

				//triang_transformed.color = get_shaded_color(std::max(0.1f, vector_dot_product(light_direction, normal)), triang_transformed.color);

				//disp::pixel_depth(10, 10, 10,  10.0f);

				triang_projected.points[0] = multiply_matrix_vect(matr_view, triang_projected.points[0]);
				triang_projected.points[1] = multiply_matrix_vect(matr_view, triang_projected.points[1]);
				triang_projected.points[2] = multiply_matrix_vect(matr_view, triang_projected.points[2]);
				
				int n_clipped_triangles = 0;
				triangle clipped[2];
				n_clipped_triangles = triangle_clips_against_plane({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f}, triang_projected, clipped[0], clipped[1]);

				for(int n = 0; n < n_clipped_triangles; n++){



				//	triang_projected.points[0] = multiply_matrix_vect(matProj, triang_viewed.points[0]);
				//	triang_projected.points[1] = multiply_matrix_vect(matProj, triang_viewed.points[1]);
				//	triang_projected.points[2] = multiply_matrix_vect(matProj, triang_viewed.points[2]);
				//triang_projected.color = triang_transformed.color;

					triang_projected.points[0] = multiply_matrix_vect(matProj, clipped[n].points[0]);
					triang_projected.points[1] = multiply_matrix_vect(matProj, clipped[n].points[1]);
					triang_projected.points[2] = multiply_matrix_vect(matProj, clipped[n].points[2]);

					triang_projected.points[0] = vector_div(triang_projected.points[0], triang_projected.points[0].w);
					triang_projected.points[1] = vector_div(triang_projected.points[1], triang_projected.points[1].w);
					triang_projected.points[2] = vector_div(triang_projected.points[2], triang_projected.points[2].w);
				//pspDebugScreenPrintf("%s", "DONE THIRD MULTIPL\n");

					vec_3d voffset_view = {1, 1, 0};

					vector_add_self(triang_projected.points[0], voffset_view);
					vector_add_self(triang_projected.points[1], voffset_view);
					vector_add_self(triang_projected.points[2], voffset_view);


			//pspDebugScreenPrintf("%s", "DONE INCREMENTS\n");

					triang_projected.points[0].x *= 240.0f;//0.5f * (float)(480);
					triang_projected.points[0].y *= 166.0f;//0.5f * (float)(272);
					triang_projected.points[1].x *= 240.0f;//0.5f * (float)(480);
					triang_projected.points[1].y *= 166.0f;//0.5f * (float)(272);
					triang_projected.points[2].x *= 240.0f;//0.5f * (float)(480);
					triang_projected.points[2].y *= 166.0f;//0.5f * (float)(272);

				//pspDebugScreenPrintf("%s", "DONE LATES SCALINGS\n");

					triang_projected.color = get_shaded_color(std::max((normal.x*light_direction.x + normal.y*light_direction.y + normal.z*light_direction.z), 0.1f), color);


					/*if(triang_projected.points[0].x >= 0 and triang_projected.points[0].x < 480 and triang_projected.points[0].y >= 0 and triang_projected.points[0].y < 272){
						if(triang_projected.points[1].x >= 0 and triang_projected.points[1].x < 480 and triang_projected.points[1].y >= 1 and triang_projected.points[1].y < 272){
							if(triang_projected.points[2].x >= 0 and triang_projected.points[2].x < 480 and triang_projected.points[2].y >= 0 and triang_projected.points[2].y < 272){
								fill_triangle_depth((int)triang_projected.points[0].x, (int)triang_projected.points[0].y, (int)triang_projected.points[1].x, (int)triang_projected.points[1].y, (int)triang_projected.points[2].x, (int)triang_projected.points[2].y, get_shaded_color(std::max((normal.x*light_direction.x + normal.y*light_direction.y + normal.z*light_direction.z), 0.1f), color), (triang_projected.points[0].z + triang_projected.points[1].z + triang_projected.points[2].z));
							}
						}
					}*/
					triangle clipped_2[2];
					std::list<triangle> listTriangles;

			// Add initial triangle
					listTriangles.push_back(triang_projected);
					int nNewTriangles = 1;
					for (int p = 0; p < 4; p++){
						int nTrisToAdd = 0;
						while (nNewTriangles > 0){
							// Take triangle from front of queue
							triangle test = listTriangles.front();
							listTriangles.pop_front();
							nNewTriangles--;

							// Clip it against a plane. We only need to test each 
							// subsequent plane, against subsequent new triangles
							// as all triangles after a plane clip are guaranteed
							// to lie on the inside of the plane. I like how this
							// comment is almost completely and utterly justified
							switch (p){
								case 0:	nTrisToAdd = triangle_clips_against_plane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped_2[0], clipped_2[1]); break;
								case 1:	nTrisToAdd = triangle_clips_against_plane({ 0.0f, 271.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped_2[0], clipped_2[1]); break;
								case 2:	nTrisToAdd = triangle_clips_against_plane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped_2[0], clipped_2[1]); break;
								case 3:	nTrisToAdd = triangle_clips_against_plane({ 479.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped_2[0], clipped_2[1]); break;
							}

							// Clipping may yield a variable number of triangles, so
							// add these new ones to the back of the queue for subsequent
							// clipping against next planes
							for (int w = 0; w < nTrisToAdd; w++){
								listTriangles.push_back(clipped_2[w]);
							}
						}
						nNewTriangles = listTriangles.size();
					}
					for(auto& t : listTriangles){
						fill_triangle_depth(t.points[0].x, t.points[0].y, t.points[1].x, t.points[1].y, t.points[2].x, t.points[2].y, triang_projected.color, (t.points[0].z + t.points[1].z + t.points[2].z));

						//draw_triangle(t.points[0].x, t.points[0].y, t.points[1].x, t.points[1].y, t.points[2].x, t.points[2].y, 0x00000000);


					}
				}
			}
		}
		disp::clear_z_buffer();
	}

	//inline void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) noexcept{
		/*disp::bresenham((int)x1, (int)y1, (int)x2, (int)y2, color);
		disp::bresenham((int)x2, (int)y2, (int)x3, (int)y3, color);
		disp::bresenham((int)x3, (int)y3, (int)x1, (int)y1, color);*/
	//	disp::draw_line((int)x1, (int)y1, (int)x2, (int)y2, 0);
	//	disp::draw_line((int)x2, (int)y2, (int)x3, (int)y3, 0);
	//	disp::draw_line((int)x1, (int)y1, (int)x3, (int)y3, 0);
		//pspDebugScreenPrintf("%s", "START OF TRIANGLE: \n");
		//pspDebugScreenPrintf("%d %d %s %d %d %c", (int)x1, (int)y1, " ", (int)x2, (int)y2, '\n');
		//pspDebugScreenPrintf("%d %d %s %d %d %c", (int)x2, (int)y2, " ", (int)x3, (int)y3, '\n');
		//pspDebugScreenPrintf("%d %d %s %d %d %c", (int)x3, (int)y3, " ", (int)x1, (int)y1, '\n');
		//pspDebugScreenPrintf("%s", "END OF TRIANGLE \n");
	//}

	inline void draw_triangle_depth(float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color, float depth) noexcept{
		//disp::bresenham_depth((int)x1, (int)y1, (int)x2, (int)y2, color, depth);
		//disp::bresenham_depth((int)x2, (int)y2, (int)x3, (int)y3, color, depth);
		//disp::bresenham_depth((int)x3, (int)y3, (int)x1, (int)y1, color, depth);

		disp::draw_line_depth((int)x1, (int)y1, (int)x2, (int)y2, color, depth);
		disp::draw_line_depth((int)x2, (int)y2, (int)x3, (int)y3, color, depth);
		disp::draw_line_depth((int)x3, (int)y3, (int)x1, (int)y1, color, depth);

		//pspDebugScreenPrintf("%s", "START OF TRIANGLE: \n");
		//pspDebugScreenPrintf("%d %d %s %d %d %c", (int)x1, (int)y1, " ", (int)x2, (int)y2, '\n');
		//pspDebugScreenPrintf("%d %d %s %d %d %c", (int)x2, (int)y2, " ", (int)x3, (int)y3, '\n');
		//pspDebugScreenPrintf("%d %d %s %d %d %c", (int)x3, (int)y3, " ", (int)x1, (int)y1, '\n');
		//pspDebugScreenPrintf("%s", "END OF TRIANGLE \n");
	}
};

long long int current_timestamp() noexcept{
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}