#ifndef PBR_PIPELINE_H
#define PBR_PIPELINE_H

#include "../pipeline/pipeline.h"

typedef struct {
  Pipeline pipeline;
} PbrPipeline;

PbrPipeline create_pipeline();

#endif /* PBR_PIPELINE_H */
