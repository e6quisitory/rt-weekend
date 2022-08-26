
#include "renderer.h"
#include "matte.h"
#include "metal.h"
#include "sphere.h"

int main() {

  /* Initialize renderer */
  renderer r;

  /* Camera setup */
  point3 lookfrom = point3(-2, 0, 1);
  point3 lookat = point3(-2,0,-1);
  point3 focusat = lookat;

  r.cam = camera(lookfrom, lookat, focusat, y_hat(), 16.0/9.0, 70, 0.25);

  /* Materials specifications */
  auto material_ground = std::make_shared<matte>(color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<matte>(color(0.4));
  auto material_left   = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);
  auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.2);

  /* Create world and add shaded objects */
  r.world = hittable_list();
  auto l = std::cos(3.14159/4);
  r.world.add(std::make_shared<sphere>(point3( 0.0, -100.5*l, -1.0), 100.0*l, material_ground));
  r.world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.45*l, material_center));
  r.world.add(std::make_shared<sphere>(point3(-l,    0.0, -1.0),   0.5*l, material_left));
  r.world.add(std::make_shared<sphere>(point3( l,    0.0, -1.0),   0.5*l, material_right));

  /* Render quality specifications */
  r.core_count = std::thread::hardware_concurrency();
  r.samples_per_pixel = 50;
  r.bounce_depth = 50;

  /* Output file specifications */
  r.image_width = 400;
  r.image_height = r.image_width / r.cam.aspect_ratio; // image & viewport have same aspect ratio

  /* Render */
  r.render_straight_line(point3(0,0,1), video_params(1, 30));

  spinning_circle_params scp = {
    x_hat(),
    -z_hat(),
    point3(0,0,-1),
    2*pi + 2.8*((2*pi/4)/4),
    video_params(1, 30)
  };

  r.render_spinning_circle(scp);
  r.render_straight_line(ray(r.cam.origin, point3(0,0,-1) - r.cam.origin).at(0.27), video_params(1, 30));

  r.render_shifting_focus(r.cam.focus_dist*r.cam.view_dir, point3(l,0,-1), video_params(3, 30));

  // The output frames can be combined into an mp4 with the follwoing command: ffmpeg -framerate 30 -i "%01d.ppm" output.mp4

  return 0;
  
}
