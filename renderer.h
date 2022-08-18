#ifndef RENDERER_H
#define RENDERER_H

class renderer {
  public:
  	renderer() {}

  	void render_to_file(const std::string filename);

  private:
  	color ray_color(const ray& r, int depth);
  	void render_to_mem(pixel* image);

  public:
    hittable_list world;
    camera cam;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int bounce_depth;
    int core_count;
};

/* Takes in a ray and a list of hittable objects and returns color of the object that was hit */
color renderer::ray_color(const ray& r, int depth) {
  // If bounce depth has been reached, return black color
  if (depth <= 0) return color(0,0,0);

  hit_record rec;
  ray reflected_ray;

  if (world.hit(r, 0.001, DBL_MAX, rec)) {
    reflected_ray = rec.material_ptr->scatter(r, rec); 
    return rec.material_ptr->albedo * ray_color(reflected_ray, depth-1);
  }
  return color(1,1,1);
}

/* Renders image into an array of pixels in memory (splits samples_per_pixel across # of threads) */
void renderer::render_to_mem(pixel* image) {
  int divided_spp = samples_per_pixel/core_count;
  for (int i = image_height; i > 0; --i) {
    for (int j = 0; j < image_width; ++j) {
      color sum;
      for (int k = 0; k < divided_spp; ++k) {
          double u = (j+random_double()) / image_width;
          double v = (i+random_double()) / image_height;
          ray r = cam.get_ray(u, v);
          sum += ray_color(r, bounce_depth);
      }
      image[(image_height-i)*image_width + j] = sqrt(sum/divided_spp);  // sqrt for gamma correction
    }
  }
}

/* Renders image into a file (multithreaded) */
void renderer::render_to_file(const std::string filename) {

  /* Create output image stream */
  std::ofstream img;
  img.open(filename);
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  auto start_time = Time::now();
 
  // create an array of images (pixel arrays), one for each CPU core
  pixel* images[core_count];

  // create an array of threads
  std::thread threads[core_count-1];

  // initialize images (set the size of the arrays)
  for (int i = 0; i < core_count; ++i)
    images[i] = new pixel[image_height*image_width];

  // launch core_count-1 threads for image rendering
  for (int i = 0; i < core_count-1; ++i)
    threads[i] = std::thread(&renderer::render_to_mem, this, images[i]);

  // render image on this thread
  render_to_mem(images[core_count-1]);

  // join launched threads
  for (int i = 0; i < core_count-1; ++i)
    threads[i].join();

  // average all the images
  for (int i = 0; i < image_height*image_width; ++i) {
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
}

#endif