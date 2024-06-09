#ifndef IBL_H
#define IBL_H

#include "texture.h"

Texture ibl_generate_irradiance(const char *file, int resolution,
                                Texture *env_texture, Texture *prefilter_texture,Texture* brdf_texture);

#endif /* IBL_H */
