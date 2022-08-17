#include "includes.h"

/* Takes in a ray and a list of hittable objects and returns color of the object that was hit */
color ray_color(const ray& r, const hittable& world, int depth) {
  /* If bounce depth has been reached, return black color */
  if (depth <= 0) return color(0,0,0);

  hit_record rec;
  ray reflected_ray;

  if (world.hit(r, 0.001, DBL_MAX, rec)) {
    reflected_ray = rec.material_ptr->scatter(r, rec); 
    return rec.material_ptr->albedo * ray_color(reflected_ray, world, depth-1);
  }
  return color(1,1,1);
}

int main() {

  /* Camera setup */
  point3 lookfrom = point3(1.2, 0, -0.1);
  point3 lookat = point3(0,0,-1);
  point3 focusat = point3(-std::cos(3.14159/4), 0.0, -1.0);
  vec3 vup = vec3(0,1,0);
  camera cam(lookfrom, lookat, focusat, vup, 16.0/9.0, 70, 0.35);

  /* Materials specifications */
  auto material_ground = std::make_shared<matte>(color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<matte>(color(0.4));
  auto material_left   = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.7);
  auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

  /* Create world and add shaded objects */
  hittable_list world;
  auto l = std::cos(3.14159/4);
  world.add(std::make_shared<sphere>(point3( 0.0, -100.5*l, -1.0), 100.0*l, material_ground));
  world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.45*l, material_center));
  world.add(std::make_shared<sphere>(point3(-l,    0.0, -1.0),   0.5*l, material_left));
  world.add(std::make_shared<sphere>(point3( l,    0.0, -1.0),   0.5*l, material_right));

  /* Setup PPM file */
  const int image_width = 500;
  const int image_height = image_width / cam.aspect_ratio; // image & viewport have same aspect ratio
  std::ofstream img;
  img.open("render.ppm");
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  const int samples_per_pixel = 100;
  const int bounce_depth = 50;

  auto start_time = Time::now();
 
  for (int i = image_height; i > 0; --i) { // must go from top to bottom as PPM file starts in top left corner, not bottom
    std::cout << "\rScanlines remaining: " << i-1 << ' ' << std::flush;
    for (int j = 0; j < image_width; ++j) {
      color sum(0,0,0);
      for (int k = 0; k < samples_per_pixel; ++k) {
        double u = (j+random_double()) / image_width;
        double v = (i+random_double()) / image_height;
        ray r = cam.get_ray(u, v);
        sum += ray_color(r, world, bounce_depth);
      }
     write_color(img, sqrt((sum/samples_per_pixel))); // sqrt for gamma correction
    }
  }

  print_render_time(Time::now() - start_time, std::cout, 3);

  img.close();
  return 0;

}
