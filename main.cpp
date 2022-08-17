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
  int samples_per_pixel;
  int bounce_depth;
};

/* Takes in params struct and renders entire image into provided pixels array */
void render_image(const renderer_params& rp, pixel* image) {
  for (int i = rp.image_height; i > 0; --i) {
    for (int j = 0; j < rp.image_width; ++j) {
      color sum;
      for (int k = 0; k < rp.samples_per_pixel; ++k) {
          double u = (j+random_double()) / rp.image_width;
          double v = (i+random_double()) / rp.image_height;
          ray r = rp.cam.get_ray(u, v);
          sum += ray_color(r, rp.world, rp.bounce_depth);
      }
      image[(rp.image_height-i)*rp.image_width + j] = sqrt(sum/rp.samples_per_pixel);  // sqrt for gamma correction
    }
  }
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
  const auto core_count = std::thread::hardware_concurrency();
  rp.samples_per_pixel = 100/core_count;
  rp.bounce_depth = 50;

  auto start_time = Time::now();
 
  // create an array of images (pixel arrays), one for each CPU core
  pixel* images[core_count];

  // create an array of threads
  std::thread threads[core_count-1];

  // initialize images (set the size of the arrays)
  for (int i = 0; i < core_count; ++i)
    images[i] = new pixel[rp.image_height*rp.image_width];

  // launch core_count-1 threads for image rendering
  for (int i = 0; i < core_count-1; ++i)
    threads[i] = std::thread(render_image, std::cref(rp), images[i]);

  // render image on this thread
  render_image(rp, images[core_count-1]);

  // join launched threads
  for (int i = 0; i < core_count-1; ++i)
    threads[i].join();

  // average all the images
  for (int i = 0; i < rp.image_height*rp.image_width; ++i) {
    color sum;
    for (int j = 0; j < core_count; ++j)
      sum += images[j][i];
    write_color(img, sum/core_count);
  }

  // delete pixel arrays (images) out of memory
  for (int i = 0; i < core_count; ++i)
    delete images[i];

  print_render_time(Time::now() - start_time, std::cout, 3);

  img.close();
  return 0;

}
