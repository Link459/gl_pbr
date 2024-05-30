#ifndef PBR_PIPELINE_H
#define PBR_PIPELINE_H

#include "../pipeline/pipeline.h"

typedef struct {
  Pipeline pipeline;
} PbrPipeline;

PbrPipeline pbr_pipeline_create();
void pbr_pipeline_destroy(const PbrPipeline *);
void pbr_pipeline_bind(const PbrPipeline *pipeline);

#endif /* PBR_PIPELINE_H */
