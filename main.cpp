#include "rtweekend.h"

int main() {

  /* Initialize renderer */
  renderer r;

  /* Camera setup */
  point3 lookfrom = point3(1.2, 0, -0.1);
  point3 lookat = point3(0,0,-1);
  point3 focusat = point3(-std::cos(3.14159/4), 0.0, -1.0);
  vec3 vup = vec3(0,1,0);
  r.cam = camera(lookfrom, lookat, focusat, vup, 16.0/9.0, 70, 0.35);

  /* Materials specifications */
  auto material_ground = std::make_shared<matte>(color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<matte>(color(0.4));
  auto material_left   = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.7);
  auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

  /* Create world and add shaded objects */
  r.world = hittable_list();
  auto l = std::cos(3.14159/4);
  r.world.add(std::make_shared<sphere>(point3( 0.0, -100.5*l, -1.0), 100.0*l, material_ground));
  r.world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.45*l, material_center));
  r.world.add(std::make_shared<sphere>(point3(-l,    0.0, -1.0),   0.5*l, material_left));
  r.world.add(std::make_shared<sphere>(point3( l,    0.0, -1.0),   0.5*l, material_right));

  /* Render quality specifications */
  r.core_count = std::thread::hardware_concurrency();
  r.samples_per_pixel = 100;
  r.bounce_depth = 50;

  /* Output file specifications */
  r.image_width = 500;
  r.image_height = r.image_width / r.cam.aspect_ratio; // image & viewport have same aspect ratio

  r.render_to_file("render.ppm");

  return 0;

}
