#include "unit_tests/suite/lumix_unit_tests.h"

#include "core/sphere.h"
#include "core/vec3.h"
#include "core/frustum.h"
#include "core/timer.h"
#include "core/log.h"

#include "core/MTJD/manager.h"

#include "graphics/culling_system.h"

namespace
{

	struct TestFrustum
	{
		Lumix::Vec3 pos;
		Lumix::Vec3 dir;
		Lumix::Vec3 up;
		float fov;
		float ratio;
		float near;
		float far;
	};

	TestFrustum test_frustum =
	{
		{ 0.f, 0.f, -5.f },
		{ 0.f, 0.f, -1.f },
		{ 0.f, 1.f, 0.f },
		60.f,
		2.32378864f,
		10.f,
		100.f
	};

	void UT_culling_system(const char* params)
	{
		Lumix::Array<Lumix::Sphere> spheres;
		for (float i = 0.f; i < 30000000.0f; i += 15.f)
		{
			spheres.push(Lumix::Sphere(i, 0.f, 50.f, 5.f));
		}

		Lumix::Frustum clipping_frustum;
		clipping_frustum.compute(
			test_frustum.pos,
			test_frustum.dir,
			test_frustum.up,
			test_frustum.fov,
			test_frustum.ratio,
			test_frustum.near,
			test_frustum.far);

		Lumix::CullingSystem* culling_system;
		{
			Lumix::MTJD::Manager mtjd_manager;

			culling_system = Lumix::CullingSystem::create(mtjd_manager);
			culling_system->insert(spheres);

			Lumix::ScopedTimer timer("Culling System");

			culling_system->cullToFrustum(clipping_frustum);
			const Lumix::CullingSystem::Results& result = culling_system->getResult();

			//Lumix::g_log_info.log("unit") << "Culling system test took: " << timer.getTimeSinceStart();

			for (int i = 0; i < result.size(); i++)
			{
				LUMIX_EXPECT_EQ(result[i], i < 6 ? result[i] : -1);
			}

		}

		Lumix::CullingSystem::destroy(*culling_system);

	}

	void UT_culling_system_async(const char* params)
	{
		Lumix::Array<Lumix::Sphere> spheres;
		for (float i = 0.f; i < 30000000.f; i += 15.f)
		{
			spheres.push(Lumix::Sphere(i, 0.f, 50.f, 5.f));
		}

		Lumix::Frustum clipping_frustum;
		clipping_frustum.compute(
			test_frustum.pos,
			test_frustum.dir,
			test_frustum.up,
			test_frustum.fov,
			test_frustum.ratio,
			test_frustum.near,
			test_frustum.far);

		Lumix::CullingSystem* culling_system;
		{
			Lumix::MTJD::Manager mtjd_manager;

			culling_system = Lumix::CullingSystem::create(mtjd_manager);
			culling_system->insert(spheres);

			Lumix::ScopedTimer timer("Culling System Async");

			culling_system->cullToFrustumAsync(clipping_frustum);

			const Lumix::CullingSystem::Results& result = culling_system->getResultAsync();
			//Lumix::g_log_info.log("unit") << "Culling system test async took: " << timer.getTimeSinceStart();

			for (int i = 0; i < result.size(); i++)
			{
				LUMIX_EXPECT_EQ(result[i], i < 6 ? result[i] : -1);
			}

		}

		Lumix::CullingSystem::destroy(*culling_system);
	}
}

REGISTER_TEST("unit_tests/graphics/culling_system", UT_culling_system, "");
REGISTER_TEST("unit_tests/graphics/culling_system_async", UT_culling_system_async, "");
