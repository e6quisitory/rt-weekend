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

struct renderer_params {
  hittable_list world;
  camera cam;
  int image_width;
  int image_height;
  int bounce_depth;
};

// tell this function with pixel you're on and it'll deal with sending the rays and give you back the final averaged pixel color
void calculate_avg_color (int i, int j, int samples_per_pixel, const renderer_params& rp, color& final_color) {
  color sum = color(0,0,0);
  for (int k = 0; k < samples_per_pixel; ++k) {
    double u = (j+random_double()) / rp.image_width;
    double v = (i+random_double()) / rp.image_height;
    ray r = rp.cam.get_ray(u, v);
    sum += ray_color(r, rp.world, rp.bounce_depth);
  }
  final_color = sqrt(sum/samples_per_pixel);   // sqrt for gamma correction
}

int main() {

  /* Initialize struct to pass into each thread (to avoid large number of args)*/
  renderer_params rp;

  /* Camera setup */
  point3 lookfrom = point3(1.2, 0, -0.1);
  point3 lookat = point3(0,0,-1);
  point3 focusat = point3(-std::cos(3.14159/4), 0.0, -1.0);
  vec3 vup = vec3(0,1,0);
  rp.cam = camera(lookfrom, lookat, focusat, vup, 16.0/9.0, 70, 0.35);

  /* Materials specifications */
  auto material_ground = std::make_shared<matte>(color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<matte>(color(0.4));
  auto material_left   = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.7);
  auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

  /* Create world and add shaded objects */
  rp.world = hittable_list();
  auto l = std::cos(3.14159/4);
  rp.world.add(std::make_shared<sphere>(point3( 0.0, -100.5*l, -1.0), 100.0*l, material_ground));
  rp.world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.45*l, material_center));
  rp.world.add(std::make_shared<sphere>(point3(-l,    0.0, -1.0),   0.5*l, material_left));
  rp.world.add(std::make_shared<sphere>(point3( l,    0.0, -1.0),   0.5*l, material_right));

  /* Setup PPM file */
  rp.image_width = 500;
  rp.image_height = rp.image_width / rp.cam.aspect_ratio; // image & viewport have same aspect ratio
  std::ofstream img;
  img.open("render.ppm");
  img << "P3\n" << rp.image_width << ' ' << rp.image_height << "\n255\n"; 

  /* Render image */
  const int samples_per_pixel = 100;
  rp.bounce_depth = 50;
  const auto core_count = std::thread::hardware_concurrency();

  auto start_time = Time::now();
 
  for (int i = rp.image_height; i > 0; --i) { // must go from top to bottom as PPM file starts in top left corner, not bottom
    std::cout << "\rScanlines remaining: " << i-1 << ' ' << std::flush;
    for (int j = 0; j < rp.image_width; ++j) {

      color pixels[core_count];
      std::thread threads[core_count-1];

      for (int c = 0; c < core_count; ++c)
        pixels[c] = color(0,0,0);

      calculate_avg_color(i, j, samples_per_pixel/core_count, rp, pixels[core_count-1]);

      for (int c = 0; c < core_count-1; ++c)
        threads[c] = std::thread(calculate_avg_color, i, j, samples_per_pixel/core_count, std::cref(rp), std::ref(pixels[c]));

      for (int c = 0; c < core_count-1; ++c)
        threads[c].join();

      color final_sum = color(0,0,0);
      for (int c = 0; c < core_count; ++c) 
        final_sum += pixels[c];

      write_color(img, final_sum/core_count);
    
    }
  }

  print_render_time(Time::now() - start_time, std::cout, 3);

  img.close();
  return 0;

}
