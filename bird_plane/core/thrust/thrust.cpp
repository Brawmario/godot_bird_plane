#include "api.hpp"

static float velocity = 0;
static float thrust = 300;
static float thrust_limit = 500;
static float thrust_delta = 0;
static float gravity_influence = 100;
static float smooth = 0.9;

static Variant _physics_process_plane(double delta, Node3D plane) {
	if (is_editor_hint()) {
		return Nil;
	}

	static constexpr double Math_PI = 3.14159265358979;
	const Basis basis = plane.get_transform().get_basis();
	const Vector3 direction = basis.get_row(2);

	const float y_ang = direction.angle_to(Vector3{0, 1, 0});
	if ((thrust <= thrust_limit && thrust_delta > 0) || (thrust >= 0 && thrust_delta < 0)) {
		thrust += thrust_delta;
	}

	const float target_speed = thrust + (y_ang / Math_PI - 0.5) * gravity_influence; // We subtract 0.5 from the angle over pi because gravity's influence should be zero at 90 degrees or when the angle over pi is 0.5
	velocity = (velocity * smooth + target_speed * (1 - smooth)) / 2;

	const Vector3 target_vector {0, 0, velocity};
	Transform3D transform = plane.get_transform();
	transform = transform.translated_local(target_vector * delta);
	plane.set_transform(transform);

	return Nil;
}

int main() {
	ADD_PROPERTY(velocity, Variant::FLOAT);
	ADD_PROPERTY(thrust, Variant::FLOAT);
	ADD_PROPERTY(thrust_limit, Variant::FLOAT);
	ADD_PROPERTY(thrust_delta, Variant::FLOAT);
	ADD_PROPERTY(gravity_influence, Variant::FLOAT);
	ADD_PROPERTY(smooth, Variant::FLOAT);

	ADD_API_FUNCTION(_physics_process_plane, "void", "double delta, Node3D plane", "Thrust calculations");
}
