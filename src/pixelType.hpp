#ifndef HPP_PIXELTYPE
#define HPP_PIXELTYPE

typedef enum{
  pixelType_AIR,
  pixelType_DIRT,
  pixelType_STONE,
  pixelType_SEED,
  pixelType_LEAVES,
  pixelType_FRUIT,
  pixelType_WATER,
  pixelType_TOTAL
}pixelType;

typedef enum{
  pixelGravity_CANT_FALL,
  pixelGravity_MAY_FALL,
  pixelGravity_NOT_RELEVANT
}pixelGravity;

typedef enum{
  pixelPhysicalState_GASEOUS,
  pixelPhysicalState_LIQUID,
  pixelPhysicalState_SOLID
}pixelPhysicalState;

#endif
