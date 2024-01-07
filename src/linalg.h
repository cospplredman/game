
typedef struct {
	float x, y, z;
} vec3;

typedef struct {
	float dat[4][4];
} mat4;

static inline vec3 add(vec3 a, vec3 b){
	return (vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }

static inline vec3 sub(vec3 a, vec3 b){
	return (vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }

static inline vec3 mul(vec3 a, vec3 b){
	return (vec3){a.x * b.x, a.y * b.y, a.z * b.z}; }

static inline float dot(vec3 a, vec3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z; }


