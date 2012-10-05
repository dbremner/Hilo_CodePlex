#pragma once

struct AmpPixel;

void ApplyCartoonEffectAmp(AmpPixel* sourcePixels, 
                           AmpPixel* destinationPixels,
                           unsigned int width, 
                           unsigned int height, 
                           unsigned int neighborWindow, 
                           unsigned int phases, 
                           concurrency::cancellation_token token);