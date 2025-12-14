#pragma once
#include "DrawDebugHelpers.h" // Header files include draw debug functions

// Persistant Draw Version
#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Red,true);
#define DRAW_LINE(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,true,-1.f,0,1.f)
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(),Location,10.f,FColor::Red,true)
#define DRAW_VECTOR(StartLocation,EndLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,true,-1.f,0,1.f); \
		DrawDebugPoint(GetWorld(),EndLocation,10.f,FColor::Red,true); \
	}

// Draw Version for Tick
#define DRAW_LINE_PerFrame(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,false,-1.f,0,1.f)
#define DRAW_SPHERE_PerFrame(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Red,false, -1.f);
#define DRAW_POINT_PerFrame(Location) if(GetWorld()) DrawDebugPoint(GetWorld(),Location,10.f,FColor::Red,false, -1.f)
#define DRAW_VECTOR_PerFrame(StartLocation,EndLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,false,-1.f,0,1.f); \
		DrawDebugPoint(GetWorld(),EndLocation,10.f,FColor::Red,false, -1.f); \
	}

// Draw Version with Duration
#define DRAW_SPHERE_Duration(Location, Duration) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 12, FColor::Red, false, Duration);
#define DRAW_LINE_Duration(StartLocation, EndLocation, Duration) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, Duration, 0, 1.f);
#define DRAW_POINT_Duration(Location, Duration) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 10.f, FColor::Red, false, Duration);
#define DRAW_VECTOR_Duration(StartLocation, EndLocation, Duration) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, Duration, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 10.f, FColor::Red, false, Duration); \
	}


