#include "./lib/scene.h"
#include "./lib/render.h"

typedef struct {
	float t;
	material mat;
	ray normal;
} rayHit;

rayHit raySphereHit(ray curRay, sphere curSphere)
{
	float temp[2][3];
  vecSub(temp[0], curRay.pos, curSphere.pos);
  float bottom = dotProd(curRay.dir, curRay.dir);
	float left = dotProd(vecMult(temp[1], curRay.dir, -1), temp[0]);
	float right = pow(dotProd(curRay.dir, temp[0]), 2.0) - bottom * (dotProd(temp[0], temp[0]) - pow(curSphere.radius, 2.0));

  float solution[2];
	solution[0] = (left - sqrt(right)) / bottom;
	solution[1] = (left + sqrt(right)) / bottom;

  // Create a hit that currently says no hit
  rayHit sphereHit;
  sphereHit.t = -1;

  // If their is no hit or we are in a sphere or it is behind us return no hit
  if(right < 0 || solution[0] < 0 || solution[1] < 0)
    return sphereHit;

  // Give the smaller solution as the distance from the camera
  if(solution[0] < solution[1]) {
    sphereHit.t = solution[0];
  } else {
    sphereHit.t = solution[1];
  }

  // Assign the material
  sphereHit.mat = curSphere.mat;

  // Calculate the point at which the ray hit the sphere
  vecAdd(sphereHit.normal.pos, curRay.pos, vecMult(temp[0], curRay.dir, sphereHit.t));
  // Get the direction the normal is facing
  vecSub(sphereHit.normal.dir, sphereHit.normal.pos, curSphere.pos);
  norm(&sphereHit.normal);

  // Return the rayHit
  return sphereHit;
}

rayHit rayTriangleHit(ray curRay, triangle curTriangle)
{
	float a = curTriangle.pos[0][0] - curTriangle.pos[1][0];
	float b = curTriangle.pos[0][1] - curTriangle.pos[1][1];
	float c = curTriangle.pos[0][2] - curTriangle.pos[1][2];
	float d = curTriangle.pos[0][0] - curTriangle.pos[2][0];
	float e = curTriangle.pos[0][1] - curTriangle.pos[2][1];
	float f = curTriangle.pos[0][2] - curTriangle.pos[2][2];
	float g = curRay.dir[0];
	float h = curRay.dir[1];
	float i = curRay.dir[2];
	float j = curTriangle.pos[0][0] - curRay.pos[0];
	float k = curTriangle.pos[0][1] - curRay.pos[1];
	float l = curTriangle.pos[0][2] - curRay.pos[2];
	float m = (a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g));

	// Create a hit that currently says no hit
	rayHit triangleHit;
	triangleHit.t = -1;

	float t = (-1 * (f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c))) / m;
	if(t < 0)
		return triangleHit;

	float gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / m;
	if(gamma < 0 || gamma > 1)
		return triangleHit;

	float beta = (j * (e * i - h * f) + k * (g * f - d * i) + l * (d * h - e * g)) / m;
	if(beta < 0 || beta > 1 - gamma)
		return triangleHit;

	triangleHit.t = t;
	triangleHit.mat = curTriangle.mat;

	// Calculate the point at which the ray hit the triangle
	float temp[2][3];
	vecAdd(triangleHit.normal.pos, curRay.pos, vecMult(temp[0], curRay.dir, triangleHit.t));
	// Calculate the direction of the triangle based on the winding
	xProd(triangleHit.normal.dir, vecSub(temp[0], curTriangle.pos[2], curTriangle.pos[0]), vecSub(temp[1], curTriangle.pos[1], curTriangle.pos[0]));
	norm(&triangleHit.normal);
	return triangleHit;
}

rayHit objectDetect(ray curRay, sphere* spheres, int numSpheres, triangle* triangles, int numTriangles)
{
	// ray hit for returning the closest hit or no hit
	rayHit closestHit;
	closestHit.t = -1;

	// Check all of the spheres
	for(int i = 0; i < numSpheres; i++) {
		rayHit sphereHit = raySphereHit(curRay, spheres[i]);
		// If there is a sphere hit and it is closer than the previous or there were no previous hit
		if((sphereHit.t != -1 && sphereHit.t < closestHit.t) || closestHit.t == -1) {
			// Replace the closest hit with the sphere hit
			closestHit.t = sphereHit.t;
			closestHit.mat = sphereHit.mat;
			vecMult(closestHit.normal.pos, sphereHit.normal.pos, 1);
			vecMult(closestHit.normal.dir, sphereHit.normal.dir, 1);
		}
	}

	// Check all of the triangles
	for(int i = 0; i < numTriangles; i++) {
		rayHit triangleHit = rayTriangleHit(curRay, triangles[i]);
		// If there is a triangle hit and it is closer than the previous or there were no previous hit
		if((triangleHit.t != -1 && triangleHit.t < closestHit.t) || closestHit.t == -1) {
			// Replace the closest hit with the triangle hit
			closestHit.t = triangleHit.t;
			closestHit.mat = triangleHit.mat;
			vecMult(closestHit.normal.pos, triangleHit.normal.pos, 1);
			vecMult(closestHit.normal.dir, triangleHit.normal.dir, 1);
		}
	}

	return closestHit;
}

void setPixel(camera curCam, int x, int y, int red, int green, int blue)
{
	int pixelPosition = x * curCam.channels + curCam.width * curCam.channels * y;
	curCam.data[pixelPosition] = blue;
	curCam.data[pixelPosition + 1] = green;
	curCam.data[pixelPosition + 2] = red;
  curCam.data[pixelPosition + 3] = 255;
}

int main()
{
  // Import the scene a file type that holds all the data for starting a level
  scene curScene = makeScene("./scenes/other.scene");
  camera cam = curScene.cameras[0];

  // Create the context in which to render frames
  RenderContext ctx;
  render_init(&ctx, curScene.cameras[0].width, curScene.cameras[0].height, curScene.cameras[0].channels);

  // Create a vector that points directly up so the camera never loses its roll rotation
  float upVec[3] = {0, 1, 0};
  // Do the render a frame process
  do {
    // Capture an innstance of the camera to preserve orientation per frame
    // TO DO: actually make this grab a copy not just a pointer

    // Create a ray that crosses the image plane horizontally
    ray widthRay;
  	vecMult(widthRay.pos, cam.orient.pos, 1);
    // The cam ray must be pointed at the image plane and end on it, so we multiply the dir by the planes distance
    float tempp[3];
  	xProd(widthRay.dir, upVec, vecMult(tempp, cam.orient.dir, cam.planeDist));
    // Normalize and multiply the ray by half the plane width to make it end on the edge of the image plane
  	norm(&widthRay);
  	vecMult(widthRay.dir, widthRay.dir, cam.planeWidthHalf);

    // Create a ray that crosses the image plane vertically
    ray heightRay;
  	vecMult(heightRay.pos, cam.orient.pos, 1);
    // The cam ray has already been multiplied so we dont do it again
  	xProd(heightRay.dir, cam.orient.dir, widthRay.dir);
    // Normalize and multiply the ray by half the plane height to make it end on the edge of the image plane
  	norm(&heightRay);
  	vecMult(heightRay.dir, heightRay.dir, cam.planeHeightHalf);

    // Create the image by doing linear algebra for each pixel width first then height starting in the top left
    for (int y = 0; y < cam.height; ++y) {
      for (int x = 0; x < cam.width; ++x) {
        // Create a vector that measures the disance from the camera look ray to the pixel horizontally
				float pixelWidthDist[3];
        vecMult(pixelWidthDist, widthRay.dir, ((cam.planeWidthHalf) - (cam.pixelWidth / 2.0) - (cam.pixelWidth * x)));

        // Create a vector that measures the disance from the camera look ray to the pixel vertically
        float pixelHeightDist[3];
        vecMult(pixelHeightDist, heightRay.dir, ((cam.planeHeightHalf) - (cam.pixelHeight / 2.0) - (cam.pixelHeight * y)));

        // Fill and normalize the current ray to be pointing throught the pixel (x,y)
        ray curRay;
        vecMult(curRay.pos, cam.orient.pos, 1);
  			vecAdd(curRay.dir, cam.orient.dir, pixelWidthDist);
  			vecAdd(curRay.dir, curRay.dir, pixelHeightDist);
  			norm(&curRay);

        // Detect if there is a hit
        // Store the closest hit
        rayHit closestHit;
  			closestHit.t = 1;

        // Allow for ten reflections
  			for(int i = 0; i < 10; i++) {
  				// Determine if curRay is intersecting any object
  				closestHit = objectDetect(curRay, curScene.spheres, curScene.sphereCount, curScene.triangles, curScene.triangleCount);
  				// Check if the current closest hit is a hit
  				if(closestHit.t != 1) {
  					// Check if the current closest hit is reflective
  					if(closestHit.mat.reflective == 1) {
  						// Move the position of the curRay and calculate its new direction
  						float temp[3];
							vecAdd(curRay.pos, closestHit.normal.pos, vecMult(temp, closestHit.normal.dir, 0.001));
  						vecAdd(curRay.dir, curRay.dir, vecMult(temp, closestHit.normal.dir, (-2 * dotProd(curRay.dir, closestHit.normal.dir))));
  						norm(&curRay);
  					} else {
  						break;
  					}
  				} else {
  					break;
  				}
  			}

  			// If their is no hit at all make the pixel black
  			if(closestHit.t != -1) {
  				// Create a ray shooting from the hit intersect to the light source.
  				ray shadowRay;
  				vecMult(shadowRay.pos, closestHit.normal.pos, 1);
  				vecMult(shadowRay.dir, curScene.lights[0].pos, 1);
  				vecSub(shadowRay.dir, shadowRay.dir, shadowRay.pos);
  				norm(&shadowRay);

  				float temp[3];
  				vecAdd(shadowRay.pos, shadowRay.pos, vecMult(temp, shadowRay.dir, 0.001));

  				// Calculate the diffuse value
  				float diffuse = dotProd(shadowRay.dir, closestHit.normal.dir);

  				// Create a ray hit to determine if there is an object along the shadowRay towards the light
  				rayHit shadowHit = objectDetect(shadowRay, curScene.spheres, curScene.sphereCount, curScene.triangles, curScene.triangleCount);

  				// Clamp diffuse to .2 and set the shadow value to .2
  				if(diffuse < .2 || shadowHit.t != -1)
  					diffuse = .2;

  				// Change the colors
  				int red = closestHit.mat.color[0] * curScene.lights[0].color[0] * diffuse * 255.0;
  				int green = closestHit.mat.color[1] * curScene.lights[0].color[1] * diffuse * 255.0;
  				int blue = closestHit.mat.color[2] * curScene.lights[0].color[2] * diffuse * 255.0;
  				setPixel(cam, x, y, red, green, blue);
  			} else {
          setPixel(cam, x, y, 87, 206, 235);
        }
      }
    }
    float temp2[3];
    vecAdd(cam.orient.pos, cam.orient.pos, vecMult(tempp, cam.orient.dir, ctx.input.move.y));
    vecAdd(cam.orient.pos, cam.orient.pos, vecMult(tempp, xProd(temp2, cam.orient.dir, upVec), ctx.input.move.x));

		vecAdd(cam.orient.dir, cam.orient.dir, vecMult(tempp, widthRay.dir, ctx.input.look.x * .02));
		vecAdd(cam.orient.dir, cam.orient.dir, vecMult(tempp, heightRay.dir, ctx.input.look.y * .02));
		ctx.input.look.x = 0;
		ctx.input.look.y = 0;
		norm(&cam.orient);

    // If the image is rendered start over
  } while (render_frame(&ctx, curScene.cameras[0].data));

  // Close the window
  render_exit(&ctx);
}
