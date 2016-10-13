#include "Wrapper.hpp"


#pragma region Context
GreatVEngine::OpenGL::Context::~Context()
{
}
#pragma endregion
#pragma region Texture
const GreatVEngine::OpenGL::Texture::Filter GreatVEngine::OpenGL::Texture::Filter::Off(Min::Off, Mag::Off);
const GreatVEngine::OpenGL::Texture::Filter GreatVEngine::OpenGL::Texture::Filter::Linear(Min::Linear, Mag::Linear);
const GreatVEngine::OpenGL::Texture::Filter GreatVEngine::OpenGL::Texture::Filter::Mipmap(Min::MipmapLinearLinear, Mag::Linear);
#pragma endregion
#pragma region Sampler
const GreatVEngine::OpenGL::Sampler::Filter GreatVEngine::OpenGL::Sampler::Filter::Off(Min::Off, Mag::Off);
const GreatVEngine::OpenGL::Sampler::Filter GreatVEngine::OpenGL::Sampler::Filter::Linear(Min::Linear, Mag::Linear);
const GreatVEngine::OpenGL::Sampler::Filter GreatVEngine::OpenGL::Sampler::Filter::Mipmap(Min::MipmapLinearLinear, Mag::Linear);
#pragma endregion


#pragma region
#pragma endregion
