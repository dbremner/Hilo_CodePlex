#pragma once

void ApplyCartoonEffectPPL(byte* sourcePixels, 
                           unsigned int width, 
                           unsigned int height, 
                           unsigned int neighborWindow, 
                           unsigned int phases, 
                           unsigned int size, 
                           concurrency::cancellation_token token);