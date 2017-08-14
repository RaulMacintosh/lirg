#include <iostream>
#include "../utility/vec3.h"
#include "../utility/ray.h"

using namespace utility;

/*
 *  In the near future we'll want to refactor our project to become
 *  something like the code below.
 */
// #ifdef TRADITIONAL_RT
// Image Raytrace (Camera cam, Scene scene, int width, int height)
// {
// 	Image image = new Image (width, height) ;
// 	for (int i = 0 ; i < height ; i++)
// 		for (int j = 0 ; j < width ; j++) {
// 			Ray ray = RayThruPixel (cam, i, j) ;
// 			Intersection hit = Intersect (ray, scene) ;
// 			image[i][j] = FindColor (hit) ;
// 			}
// 	return image ;
// }
// #endif

// bool hit_sphere(const Ray & r_, const point3 & c_, float radius_){
//     auto oc = r_.get_origin() - c_;
//     float a = dot(r_.get_direction(), r_.get_direction());
//     float b = 2.0 * dot(oc, r_.get_direction());
//     float c = dot(oc, oc) - (radius_* radius_);
//     return (b*b - 4.0*a*c) >= 0;
// }

float euclidian_distance(point3 p, point3 pc){
    float w =   pow (p.x() - pc.x(), 2) + 
                pow (p.y() - pc.y(), 2) + 
                pow (p.z() - pc.z(), 2);
    float distance = sqrt(w);
    return distance;
}

point3 p_center = point3(-1,-0.5,-1);

rgb color(const Ray & r_){

    rgb top_left (0,1,0);
    rgb bottom_left(0,0,0);
    rgb top_right (1,1,0);
    rgb bottom_right (1,0,0);

    
    // if(hit_sphere(r_, point3(0.5,0,-1), 0.5)){
    //     float h = euclidian_distance(r_.get_direction(), p_center);
    //     return rgb(1-h,1-h,1-h);
    // }

    // auto unit_ray = utility::unit_vector(r_.get_direction());
    auto unit_ray = r_.get_direction();


    point3 p1 = point3(unit_ray.x(), unit_ray.y(), unit_ray.z());
    float n = euclidian_distance(p1, p_center);
    if(n <= 0.4){
        float h = euclidian_distance(r_.get_direction(), p_center);
        return rgb(1-h,1-h,1-h);
    }

    float t = 0.5 * unit_ray.y() + 0.5;
    float u = 0.25 * unit_ray.x() + 0.5;


    // rgb result = bottom_left*(1-ty) + top_left*ty;
    rgb result = bottom_left*(1-t)*(1-u) + 
                 top_left*t*(1-u) + 
                 bottom_right*u*(1-t) + 
                 top_right*t*u;
    // rgb result = (1-tx)*(bottom_left*(1-ty) + top_left*ty) + tx*(bottom_right*(1-ty) + top_right*ty);

    // TODO: determine the background color, which is an linear interpolation between bottom->top.
    // The interpolation is based on where the ray hits the background.
    // Imagine that the background is attached to the view-plane; in other words,
    // the virtual world we want to visualize is empty!

    return result; // Stub, replace it accordingly
}

int makeImage(){
    int n_cols = 800;
    int n_rows = 400;

    std::cout << "P3\n"
              << n_cols << " " << n_rows << "\n"
              << "255\n";

    //=== Defining our 'camera'
    point3 lower_left_corner(-2, -1, -1); // lower left corner of the view plane.
    vec3 horizontal(4, 0, 0); // Horizontal dimension of the view plane.
    vec3 vertical(0, 2, 0); // Vertical dimension of the view plane.
    point3 origin(0, 0, 0); // the camera's origin.

    // NOTICE: We loop rows from bottom to top.
    // Y
    for (int row = n_rows-1; row >= 0; --row){

        // X
        for(int col = 0; col < n_cols; col++){

            // Determine how much we have 'walked' on the image: in [0,1]
            float u = float(col) / float(n_cols); // walked u% of the horizontal dimension of the view plane.
            float v = float(row) / float(n_rows); // walked v% of the vertical dimension of the view plane.

            // Determine the ray's direction, based on the pixel coordinate (col,row).
            // We are mapping (matching) the view plane (vp) to the image.
            // To create a ray we need: (a) an origin, and (b) an end point.
            //
            // (a) The ray's origin is the origin of the camera frame (which is the same as the world's frame).
            //
            // (b) To get the end point of ray we just have to 'walk' from the
            // vp's origin + horizontal displacement (proportional to 'col') +
            // vertical displacement (proportional to 'row').
            point3 end_point = lower_left_corner + u*horizontal + v*vertical ;
            // The ray:
            Ray r(origin, end_point - origin);

            // Determine the color of the ray, as it travels through the virtual space.
            rgb c = color(r);

            int ir = int(255.99f * c.r());
            int ig = int(255.99f * c.g());
            int ib = int(255.99f * c.b());

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }

    return 0;
}

int main(int argc, char* argv[]){

    int x = 0;
    int y = 0;
    if(argc > 1){
        x = atoi(argv[1]);
    }
    if(argc > 2){
        y = atoi(argv[2]);
    }

    // float ny = -1.0+(2*y*0.1);
    // if(ny >= 1.0){
    //     ny = 1.0 - (ny - 1.0);
    // }
    // if(ny < -1.0){
    //     ny = -1.0 + (-ny + 1.0);
    // }
    // p_center = point3(-2.0+(x*0.1),ny,p_center.z());
 
    p_center = point3(p_center.x()+x*0.1,p_center.y()+y*0.1,p_center.z());


    makeImage();
    // float x = -1;
    // float x = -1;
    // while

    
}