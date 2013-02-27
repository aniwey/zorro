#ifndef HPP_PIXELTYPE
#define HPP_PIXELTYPE

typedef enum{
  pixelType_INVALID,
  pixelType_NONE,
  pixelType_DIRT,
  pixelType_SEED,
  pixelType_LEAVES,
  pixelType_FRUIT,
  pixelType_TOTAL
}pixelType;

typedef enum{
  pixelForegroundType_INVALID,
  pixelForegroundType_AIR,
  pixelForegroundType_STONE,
  pixelForegroundType_WATER,
  pixelForegroundType_TOTAL
}pixelForegroundType;

typedef enum{
  pixelGravity_CANT_FALL,
  pixelGravity_MAY_FALL,
  pixelGravity_NOT_RELEVANT
}pixelGravity;

typedef enum{
  pixelForegroundPhysicalState_GASEOUS,
  pixelForegroundPhysicalState_LIQUID,
  pixelForegroundPhysicalState_SOLID
}pixelForegroundPhysicalState;

#endif
