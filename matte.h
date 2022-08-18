#ifndef MATTE_H
#define MATTE_H

class matte : public material {
  public:
    matte(color a) { albedo = a; }
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;
};

ray matte::scatter(const ray& r_in, const hit_record& rec) const {
 point3 target = rec.p + rec.normal + random_in_unit_sphere();
 return ray(rec.p, target - rec.p);
}

#endif
