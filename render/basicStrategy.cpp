#include "basicStrategy.h"

///////////////////* my extra (for TFT_eSPI (ripped of TFT_eSPI) ) /////////////

inline void drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, TFT_eSprite &canvas, uint16_t color)
{
    canvas.drawLine(x0, y0, x1, y1, color);
    canvas.drawLine(x1, y1, x2, y2, color);
    canvas.drawLine(x2, y2, x0, y0, color);
}

void pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *_img, uint8_t *data)
{

    // Pointer within original image
    uint8_t *ptro = data + ((x + y * 64) << 1);
    // Pointer within sprite image
    uint8_t *ptrs = (uint8_t *)_img + ((x + y * SCRN_WIDTH) << 1);

    while (h--)
    {
        memcpy(ptrs, ptro, (w<<1));
        ptro += 64<<1;
        ptrs += SCRN_WIDTH<<1;
    }
}

inline void repose(Vector2_u8 &a, Vector2_u8 &b)
{
    uint8_t tmp = a.x;
    a.x = b.x;
    b.x = tmp;
    tmp = a.y;
    a.y = b.y;
    b.y = tmp;
}
inline void repose(Vector2_16 &a, Vector2_16 &b)
{
    int16_t tmp = a.x;
    a.x = b.x;
    b.x = tmp;
    tmp = a.y;
    a.y = b.y;
    b.y = tmp;
}
inline void repose(Vector &a, Vector &b)
{
    float tmp = a.x;
    a.x = b.x;
    b.x = tmp;
    tmp = a.y;
    a.y = b.y;
    b.y = tmp;
    tmp = a.z;
    a.z = b.z;
    b.z = tmp;
}

inline void repose(Vector_16 &a, Vector_16 &b)
{
    int16_t tmp = a.x;
    a.x = b.x;
    b.x = tmp;
    tmp = a.y;
    a.y = b.y;
    b.y = tmp;
    tmp = a.z;
    a.z = b.z;
    b.z = tmp;
}

void IRAM_ATTR BasicRendererStrategy::pushImageTriangleToCanvas(Vector_16 v0, Vector_16 v1, Vector_16 v2, \
                                Vector2_u8 uv0, Vector2_u8 uv1, Vector2_u8 uv2, \
                                int16_t light0, int16_t light1, int16_t light2, \
                                uint16_t* _img, uint8_t* data, uint8_t texWeight)
{
    //if (data == nullptr) return;
    
    int16_t a, b, ab, y, x, last;
    int32_t w0, w1, w2;

    uint16_t precoord, coord, texCoord;

    uint16_t z_buffer_buffer;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (v0.y > v1.y) {
        repose(v0, v1); repose(uv0, uv1); transpose(light0, light1); 
    }
    if (v1.y > v2.y) {
        repose(v2, v1); repose(uv2, uv1); transpose(light2, light1);
    }
    if (v0.y > v1.y) {
        repose(v0, v1); repose(uv0, uv1); transpose(light0, light1);
    }

    // uint16_t maxX = v2.x, minX = v0.x, dx;
    // if (maxX < v1.x) maxX = v1.x;
    // if (maxX < v0.x) maxX = v0.x;
    // if (minX > v1.x) minX = v1.x;
    // if (minX > v2.x) minX = v2.x;
    // dx =(maxX - minX);
    // uint16_t* _img_buffer = (uint16_t*)alloca(dx<<1);
    
    int16_t
    dx01 = v1.x - v0.x,
    dy01 = v1.y - v0.y,
    dx02 = v2.x - v0.x,
    dy02 = v2.y - v0.y,
    dx12 = v2.x - v1.x,
    dy12 = v2.y - v1.y,
    sa   = 0,
    sb   = 0;
    int32_t
    S = (dy12 * dx02 - dx12 * dy02),
    dx12_S = 256*(dx12)/S,
    dy12_S = 256*(dy12)/S,
    dx02_S = 256*(dx02)/S,
    dy02_S = 256*(dy02)/S,
    prepreW0 = v2.x * dy12_S - v2.y * dx12_S, //I realy have no idea how to name it
    prepreW1 = v2.y * dx02_S - v2.x * dy02_S,
    preW0 = 0,
    preW1 = 0;

    uint16_t rxb, xgx;
    if (S == 0) return;

    if (v0.y == v2.y) { // Handle awkward all-on-same-line case as its own thing
        a = b = v0.x;
        if (v1.x < a)      a = v1.x;
        else if (v1.x > b) b = v1.x;
        if (v2.x < a)      a = v2.x;
        else if (v2.x > b) b = v2.x;

        preW0 = prepreW0 + y*dx12_S;
        preW1 = prepreW1 - y*dx02_S;
        coord = x + y * SCRN_WIDTH;
        for (x = a; x <= b; x++)
        {
            uint16_t color;
            if (x < 0 || x >= SCRN_WIDTH) continue;
            int16_t light = 0;

            w0 = preW0 - x*dy12_S;
            w1 = preW1 + x*dy02_S;
            w2 = 256 - w0 - w1;

            z_buffer_buffer = (v0.z * w0 + v1.z * w1 + v2.z * w2)>>8;
            if (z_buffer_buffer >= z_buffer[coord]){
                continue;
            }
            z_buffer[coord] = z_buffer_buffer;

            light = (light0 * w0 + light1 * w1 + light2 * w2)>>7;
            if (light < 0) light = 0;
            else if (light > 255) light = 255;

            // Split out and blend 5-bit red and blue channels
            rxb = TFT_BLACK & 0xF81F;
            rxb += ((color & 0xF81F) - rxb) * (light >> 2) >> 6;
            // Split out and blend 6-bit green channel
            xgx = TFT_BLACK & 0x07E0;
            xgx += ((color & 0x07E0) - xgx) * light >> 8;
            // Recombine channels
            color = (rxb & 0xF81F) | (xgx & 0x07E0);

            color = (color >> 8) | (color << 8);
            _img[(coord)] = color;
        }
        return;
    }

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (v1.y == v2.y) last = v1.y;  // Include y1 scanline
    else         last = v1.y - 1; // Skip it

    for (y = v0.y; y <= last; y++) {
        if (y >= SCRN_HEIGHT) break;

        a   = v0.x + sa / dy01;
        b   = v0.x + sb / dy02;
        sa += dx01;
        sb += dx02;

        if (y < 0) continue;

        if (a == b) continue;
        if (a > b) transpose(a, b);
        if (a >= SCRN_WIDTH) continue;
        if (b < 0) continue;
        if (a < 0) a = 0;
        if (b >= SCRN_WIDTH) b = SCRN_WIDTH;
        
        preW0 = prepreW0 + y*dx12_S;
        preW1 = prepreW1 - y*dx02_S;
        precoord = y * SCRN_WIDTH;
        
        // int16_t lightBuffer = 0;
        for (x = a; x < b; x++)
        {
            if (x < 0 || x >= SCRN_WIDTH) continue;
            int16_t light = 0;
            uint16_t color;
            coord = precoord + x;

            w0 = preW0 - x*dy12_S;
            w1 = preW1 + x*dy02_S;
            w2 = 256 - w0 - w1;
            texCoord = (((uv0.y * w0 + uv1.y * w1 + uv2.y * w2)>>8) * texWeight) + ((uv0.x * w0 + uv1.x * w1 + uv2.x * w2)>>8);

            z_buffer_buffer = ((v0.z * w0 + v1.z * w1 + v2.z * w2)>>10);
            if (z_buffer_buffer >= z_buffer[coord]){
                // log_d("z_buff: %d", z_buffer_buffer);
                continue;
            } 
            z_buffer[coord] = z_buffer_buffer;

            light = (light0 * w0 + light1 * w1 + light2 * w2)>>7;
            if (light < 20) light = 20;
            // if (light>>4 == lightBuffer>>4)
            // {
            //     _img[(coord)] = colorBuffer;
            //     continue;
            // }
            // lightBuffer = light;
            
            color = data[(texCoord)<<1]<<8 | \
                    data[((texCoord)<<1) + 1];
            // log_d("vt.y: %d", ((uv0.y * w0 + uv1.y * w1 + uv2.y * w2)>>8) * texWeight);
            
            rxb = TFT_BLACK & 0xF81F;
            rxb += ((color & 0xF81F) - rxb) * (light >> 2) >> 6;
            // Split out and blend 6-bit green channel
            xgx = TFT_BLACK & 0x07E0;
            xgx += ((color & 0x07E0) - xgx) * light >> 8;
            // Recombine channels
            color = (rxb & 0xF81F) | (xgx & 0x07E0);

            color = (color >> 8) | (color << 8);
            // _img_buffer[(x - a)] = color;
            _img[(coord)] = color;
        }
        // memcpy((uint8_t *)_img + ((a + y * SCRN_WIDTH) << 1), (uint8_t *)_img_buffer, (b - a)<<1);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - v1.y);
    sb = dx02 * (y - v0.y);
    for (; y <= v2.y; y++) {
        if (y >= SCRN_HEIGHT) break;

        a   = v1.x + sa / dy12;
        b   = v0.x + sb / dy02;
        sa += dx12;
        sb += dx02;

        if (y < 0) continue;

        if (a == b) continue;
        if (a > b) transpose(a, b);
        if (a >= SCRN_WIDTH) continue;
        if (b < 0) continue;
        if (a < 0) a = 0;
        if (b >= SCRN_WIDTH) b = SCRN_WIDTH;

        preW0 = prepreW0 + y*dx12_S;
        preW1 = prepreW1 - y*dx02_S;
        precoord = y * SCRN_WIDTH;
        // int16_t lightBuffer = 0;
        for (x = a; x < b; x++)
        {
            if (x < 0 || x >= SCRN_WIDTH) continue;
            int16_t light = 0;
            uint16_t color;
            coord = precoord + x;

            w0 = preW0 - x*dy12_S;
            w1 = preW1 + x*dy02_S;
            w2 = 256 - w0 - w1;
            texCoord = (((uv0.y * w0 + uv1.y * w1 + uv2.y * w2)>>8) * texWeight) + ((uv0.x * w0 + uv1.x * w1 + uv2.x * w2)>>8);
            
            z_buffer_buffer = ((v0.z * w0 + v1.z * w1 + v2.z * w2)>>10);
            if (z_buffer_buffer >= z_buffer[coord]){
                continue;
            }
            z_buffer[coord] = z_buffer_buffer;

            light = (light0 * w0 + light1 * w1 + light2 * w2)>>7;
            if (light < 20) light = 20;
            // if (light>>4 == lightBuffer>>4)
            // {
            //     _img[(coord)] = colorBuffer;
            //     continue;
            // }
            // lightBuffer = light;
            
            color = data[(texCoord)<<1]<<8 | \
                    data[((texCoord)<<1) + 1];
            
            rxb = TFT_BLACK & 0xF81F;
            rxb += ((color & 0xF81F) - rxb) * (light >> 2) >> 6;
            // Split out and blend 6-bit green channel
            xgx = TFT_BLACK & 0x07E0;
            xgx += ((color & 0x07E0) - xgx) * light >> 8;
            // Recombine channels
            color = (rxb & 0xF81F) | (xgx & 0x07E0);

            color = (color >> 8) | (color << 8);
            // _img_buffer[(x - a)] = color;
            _img[(coord)] = color;
        }
        // memcpy((uint8_t *)_img + ((a + y * SCRN_WIDTH) << 1), (uint8_t *)_img_buffer, (b - a)<<1);
    }
}
void BasicRendererStrategy::pushColorTriangleToCanvas(Vector2_16 v0, Vector2_16 v1, Vector2_16 v2, \
                                Vector vn0, Vector vn1, Vector vn2, \
                               const Vector &lightDir, uint16_t* _img, const uint16_t &color)
{
    int16_t a, b, y, x, last, S;
    uint8_t w0, w1, w2;
    uint8_t *ptrs;

    Vector norm;
    int16_t light = 255;
    uint16_t col;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (v0.y > v1.y) {
        repose(v0, v1); repose(vn0, vn1);
    }
    if (v1.y > v2.y) {
        repose(v2, v1); repose(vn2, vn1);
    }
    if (v0.y > v1.y) {
        repose(v0, v1); repose(vn0, vn1);
    }


    // if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    //     a = b = x0;
    //     if (x1 < a)      a = x1;
    //     else if (x1 > b) b = x1;
    //     if (x2 < a)      a = x2;
    //     else if (x2 > b) b = x2;

    //     // drawFastHLine(a, y0, b - a + 1, color);
    //     // sprite.drawFastHLine(a, y0, b - a + 1, TFT_DARKGREY);

    //     ptro = data + ((x + y * 255) << 1);
    //     ptrs = (uint8_t *)_img + ((x + y * SCRN_WIDTH) << 1);

    //     memcpy(ptrs, ptro, 2);


    //     return;
    // }

    int16_t
    dx01 = v1.x - v0.x,
    dy01 = v1.y - v0.y,
    dx02 = v2.x - v0.x,
    dy02 = v2.y - v0.y,
    dx12 = v2.x - v1.x,
    dy12 = v2.y - v1.y,
    sa   = 0,
    sb   = 0;

    
    S = (dy12 * dx02 - dx12 * dy02);
    if (S == 0) return;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (v1.y == v2.y) last = v1.y;  // Include y1 scanline
    else         last = v1.y - 1; // Skip it

    for (y = v0.y; y <= last; y++) {
        if (y < 0 || y >= SCRN_HEIGHT) continue;

        a   = v0.x + sa / dy01;
        b   = v0.x + sb / dy02;
        sa += dx01;
        sb += dx02;

        if (a == b) continue;
        if (a > b) transpose(a, b);        
        // sprite.drawFastHLine(a, y, b - a + 1, TFT_DARKGREY);
        
        // pushImageLine(a, y, b - a + 1, _img, data);
        
        for (x = a; x <= b; x++)
        {
            if (x < 0 || x >= SCRN_WIDTH) continue;

            w0 = (-dy12 * (x - v2.x) + dx12 * (y - v2.y))*255 / S; //todo optimize
            w1 = ( dy02 * (x - v2.x) - dx02 * (y - v2.y))*255 / S;
            w2 = 255 - w0 - w1;

            norm.z = vn0.z * w0 + vn1.z * w1 + vn2.z * w2;
            if (norm.z < 0) return;
            norm.x = vn0.x * w0 + vn1.x * w1 + vn2.x * w2;
            norm.y = vn0.y * w0 + vn1.y * w1 + vn2.y * w2;

            light = (norm.x * lightDir.x + norm.y * lightDir.y + norm.z * lightDir.z);
            if (light < 0) light = 0;
            
            ptrs = (uint8_t *)_img + ((x + y * SCRN_WIDTH) << 1);
            
            // col = alphaBlend(light, color, TFT_BLACK);
            col = (col >> 8) | (col << 8);
            *ptrs = col;
        }
        
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - v1.y);
    sb = dx02 * (y - v0.y);
    for (; y <= v2.y; y++) {
        if (y < 0 || y >= SCRN_HEIGHT) continue;

        a   = v1.x + sa / dy12;
        b   = v0.x + sb / dy02;
        sa += dx12;
        sb += dx02;

        if (a > b) transpose(a, b);
        // sprite.drawFastHLine(a, y, b - a + 1, TFT_DARKGREY);

        // pushImageLine(a, y, b - a + 1, _img, data);

        for (x = a; x < b + 1; x++)
        {
            if (x < 0 || x >= SCRN_WIDTH) continue;


            w0 = (-dy12 * (x - v2.x) + dx12 * (y - v2.y))*255 / S;
            w1 = ( dy02 * (x - v2.x) - dx02 * (y - v2.y))*255 / S;
            w2 = 255 - w0 - w1;

            norm.z = vn0.z * w0 + vn1.z * w1 + vn2.z * w2;
            if (norm.z < 0) return;
            norm.x = vn0.x * w0 + vn1.x * w1 + vn2.x * w2;
            norm.y = vn0.y * w0 + vn1.y * w1 + vn2.y * w2;

            light = (norm.x * lightDir.x + norm.y * lightDir.y + norm.z * lightDir.z);
            if (light < 0) light = 0;
            
            // if (light == 255)
            // {
            //     ptrs = (uint8_t *)_img + (uint16_t(x + y * SCRN_WIDTH) << 1);

            //     col = (col >> 8) | (col << 8);
            //     *ptrs = col;
            // }
            // else
            ptrs = (uint8_t *)_img + ((x + y * SCRN_WIDTH) << 1);
            
            // col = alphaBlend(light, color, TFT_BLACK);
            col = (col >> 8) | (col << 8);
            *ptrs = col;
        }

    }
}



void IRAM_ATTR BasicRendererStrategy::renderScene(std::vector<Entity*>& entities, TFT_eSPI& tft, TFT_eSprite* canvas, uint16_t** cnvsPtr) 
{
    for (uint8_t cnvsNum = 0; cnvsNum < SPLIT_SCREEN; cnvsNum++) 
    {
        Debug::renderTime = micros();
        canvas[cnvsNum].fillScreen(TFT_SKYBLUE);

        memset(z_buffer, 255, SCRN_WIDTH*SCRN_HEIGHT*sizeof(uint8_t));

        //actual rendering
        for (uint8_t entityNum = 0; entityNum < entities.size(); entityNum++) //entities
        {
            Entity* entity = entities[entityNum];
            for (uint16_t i = 0; i < entities[entityNum]->polyLength; i++) //polygons
            {
                Polygon polygon = entity->polygons[i];
                Vector_16 verts[3] = {  entity->vertices_b[polygon.v[0]],
                                        entity->vertices_b[polygon.v[1]],
                                        entity->vertices_b[polygon.v[2]]};
                //if out of screen
                if (verts[0].x < 0 && verts[0].y < 0 && verts[1].x < 0 && verts[1].y < 0 && verts[2].x < 0 && verts[2].y < 0 ||\
                verts[0].x > SCRN_WIDTH && verts[0].y > SCRN_HEIGHT && verts[1].x > SCRN_WIDTH && verts[1].y > SCRN_HEIGHT && verts[2].x > SCRN_WIDTH && verts[2].y > SCRN_HEIGHT)
                    continue;
                Vector2_u8 uv[3] = {entity->textureCoords[polygon.vt[0]],
                                    entity->textureCoords[polygon.vt[1]],
                                    entity->textureCoords[polygon.vt[2]]};
                Vector norms[3] = { (entity->normals[polygon.vn[0]]),
                                    (entity->normals[polygon.vn[1]]), 
                                    (entity->normals[polygon.vn[2]])};
                // if (norms[0].z < 0 || norms[1].z < 0 || norms[2].z < 0) continue;
                int16_t light[3] = {int16_t((norms[0].x * lightDirection.x) + (norms[0].y * lightDirection.y) + (norms[0].z * lightDirection.z)),
                                    int16_t((norms[1].x * lightDirection.x) + (norms[1].y * lightDirection.y) + (norms[1].z * lightDirection.z)),
                                    int16_t((norms[2].x * lightDirection.x) + (norms[2].y * lightDirection.y) + (norms[2].z * lightDirection.z))};
                

                // Vector normal(Vector::Normal(verts[0], verts[1], verts[2]));
                // Vector normal((norms[0].x + norms[0].y + norms[0].z)/3, (norms[1].x + norms[1].y + norms[1].z)/3, (norms[2].x + norms[2].y + norms[2].z)/3);
                
                
                // if (normal.z < 0) continue;

                // uint16_t color = TFT_WHITE;
                // float shade = normal.ScalarProd(lightDirection)/2+0.5; //todo optimize to unitvector

                // pushImage(0,0, 64, 128, cnvsPtr[cnvsNum], entity->texture);

                // canvas[cnvsNum].pushImage(0,0, 64, 64, entity->texture );

                // pushImageTriangleToCanvas(
                //     verts[0].x, verts[0].y,
                //     verts[1].x, verts[1].y,
                //     verts[2].x, verts[2].y,
                //     entities[entityNum]->polygons[polyNum].uv[0][0],entities[entityNum]->polygons[polyNum].uv[0][1],
                //     entities[entityNum]->polygons[polyNum].uv[1][0],entities[entityNum]->polygons[polyNum].uv[1][1],
                //     entities[entityNum]->polygons[polyNum].uv[2][0],entities[entityNum]->polygons[polyNum].uv[2][1],
                //     uint8_t(shade*255),
                //     cnvsPtr[cnvsNum],
                //     entities[entityNum]->texture
                // );

                // pushColorTriangleToCanvas(
                //     verts[0], verts[1], verts[2],
                //     // uv[0], uv[1], uv[2],
                //     norms[0], norms[1], norms[2],
                //     lightDirection,
                //     cnvsPtr[cnvsNum],
                //     TFT_WHITE
                // );
                
                uint32_t texOffset = 0;
                for (uint8_t t = 0; t < polygon.texid; t++)
                {
                    texOffset += entity->textureLengths[t];
                }
                pushImageTriangleToCanvas(
                    verts[0], verts[1], verts[2],
                    uv[0], uv[1], uv[2],
                    light[0], light[1], light[2],
                    cnvsPtr[cnvsNum],
                    entity->texture + (texOffset<<1),
                    entity->textureWidths[polygon.texid]
                );
#if DEBUG_MODE

                // canvas[cnvsNum].fillTriangle(
                //     verts[0].x, verts[0].y,
                //     verts[1].x, verts[1].y,
                //     verts[2].x, verts[2].y,
                //     TFT_RED//alphaBlend(shade*255, color, TFT_BLACK)
                // );

                // drawTriangle(
                //     verts[0].x, verts[0].y,
                //     verts[1].x, verts[1].y,
                //     verts[2].x, verts[2].y,
                //     canvas[cnvsNum],
                //     TFT_DARKGREY
                // );
#endif
            }
        }

        Debug::renderTimeSum += micros() - Debug::renderTime;
        Debug::renderCallsCount++;
        tft.pushImageDMA(0, SCRN_HEIGHT/SPLIT_SCREEN*cnvsNum, SCRN_WIDTH, SCRN_HEIGHT/SPLIT_SCREEN, cnvsPtr[cnvsNum]);
    }
}