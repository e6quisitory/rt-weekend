#pragma once

class camera {
  public:
    camera();
    camera(point3 lookfrom, point3 look_at, point3 focusat, vec3 vup, double aspr, double yfov, double aperature);

    void orient(point3 lookfrom, point3 lookat, vec3 vup);
    void focus(point3 focusat);
    void pan(vec3 direction, double pan_amount);
    ray get_ray(double u, double v) const;

  private:
    void set_basis(point3 lookfrom, point3 lookat, vec3 vup);
    void set_focus_distance(point3 focusat);
    void set_viewport_specs();
    void set_imageplane_vecs();
    vec3 random_in_unit_disk() const;

  public:
    double aspect_ratio;
    point3 origin;
    point3 lookat;
    vec3 y;
    point3 lower_left_corner;
    point3 top_left_corner;
    double focus_dist;
    vec3 view_dir;

  private:
    vec3 x;
    vec3 horizontal;
    vec3 vertical;
    double lens_radius;
    double y_fov;
    double viewport_height;
    double viewport_width;
};