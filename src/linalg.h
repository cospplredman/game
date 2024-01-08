
#ifndef GAME_LINALG_H
#define GAME_LINALG_H

#include<math.h>

typedef struct {
	float x, y;
} vec2;

typedef struct {
	int x, y;
} ivec2;


typedef struct {
	float x, y, z;
} vec3;

typedef struct {
	float dat[4][4];
} mat4;

static inline ivec2 add2i(ivec2 a, ivec2 b){
	return (ivec2){a.x + b.x, a.y + b.y}; }

static inline ivec2 sub2i(ivec2 a, ivec2 b){
	return (ivec2){a.x - b.x, a.y - b.y}; }

static inline ivec2 mul2i(ivec2 a, int b){
	return (ivec2){a.x * b, a.y * b}; }

static inline int dot2i(ivec2 a, ivec2 b){
	return a.x * b.x + a.y * b.y; }

static inline int len2i(ivec2 a){
	return sqrt(dot2i(a, a));}




static inline vec2 add2(vec2 a, vec2 b){
	return (vec2){a.x + b.x, a.y + b.y}; }

static inline vec2 sub2(vec2 a, vec2 b){
	return (vec2){a.x - b.x, a.y - b.y}; }

static inline vec2 mul2(vec2 a, float b){
	return (vec2){a.x * b, a.y * b}; }

static inline float dot2(vec2 a, vec2 b){
	return a.x * b.x + a.y * b.y; }

static inline float len2(vec2 a){
	return sqrt(dot2(a, a));}



static inline vec3 add(vec3 a, vec3 b){
	return (vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }

static inline vec3 sub(vec3 a, vec3 b){
	return (vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }

static inline vec3 mul(vec3 a, vec3 b){
	return (vec3){a.x * b.x, a.y * b.y, a.z * b.z}; }

static inline float dot(vec3 a, vec3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z; }

#endif 

