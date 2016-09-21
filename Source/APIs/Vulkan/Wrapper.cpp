#include "Wrapper.hpp"


#if GVE_DEBUG
#include <iostream>
#endif


#pragma region Instance
#if GVE_DEBUG
VkBool32 __stdcall GreatVEngine::Vulkan::Instance::DebugCallback(
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage,
	void*                                       pUserData)
{
	auto mode = [&]() {
		String text = "(";
		if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0) text += "INFORMATION";
		if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0) text += "WARNING";
		if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0) text += "PERFORMANCE";
		if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) text += "ERROR";
		if((flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0) text += "DEBUG";
		text += ")";
		return text;
	}();

	auto message = 
		mode
		+ "<" + pLayerPrefix + ">"
		+ "{\n" + pMessage + "\n}";

	if(pUserData != nullptr)
	{
		auto instance = reinterpret_cast<Instance*>(pUserData);

		instance->onDebug(message);
	}

	return false;
}
#endif
#pragma endregion
#pragma region Command
void GreatVEngine::Vulkan::Command::Parse(CommandBuffer* commandBuffer_)
{
}
#pragma endregion
#pragma region Commands
#pragma region BindGraphicPipeline
void GreatVEngine::Vulkan::Commands::BindGraphicPipeline::Parse(CommandBuffer* commandBuffer_)
{
	vkCmdBindPipeline(
		commandBuffer_->GetHandle(),
		VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
		graphicPipeline->GetHandle());
}
#pragma endregion
#pragma region BindVertexBuffers
void GreatVEngine::Vulkan::Commands::BindVertexBuffers::Parse(CommandBuffer* commandBuffer_)
{
	Vector<Buffer::Handle> vk_buffers(buffers.size());

	for(Size i = 0; i < buffers.size(); ++i)
	{
		vk_buffers[i] = buffers[i]->GetHandle();
	}

	vkCmdBindVertexBuffers(
		commandBuffer_->GetHandle(),
		0, vk_buffers.size(),
		vk_buffers.data(), offsets.data());
}
#pragma endregion
#pragma region BindIndexBuffer
void GreatVEngine::Vulkan::Commands::BindIndexBuffer::Parse(CommandBuffer* commandBuffer_)
{
	vkCmdBindIndexBuffer(
		commandBuffer_->GetHandle(),
		buffer->GetHandle(), offset,
		type);
}
#pragma endregion
#pragma region BindGraphicDescriptorSet
void GreatVEngine::Vulkan::Commands::BindGraphicDescriptorSet::Parse(CommandBuffer* commandBuffer_)
{
	Vector<DescriptorSet::Handle> vk_descriptorSets(descriptorSets.size());

	for(Size i = 0; i < descriptorSets.size(); ++i)
	{
		vk_descriptorSets[i] = descriptorSets[i]->GetHandle();
	}

	vkCmdBindDescriptorSets(
		commandBuffer_->GetHandle(),
		VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineLayout->GetHandle(),
		0,
		vk_descriptorSets.size(),
		vk_descriptorSets.data(),
		0,
		nullptr);
}
#pragma endregion
#pragma region RenderPass
void GreatVEngine::Vulkan::Commands::RenderPass::Parse(CommandBuffer* commandBuffer_)
{
	VkRenderPassBeginInfo vk_renderPassBeginInfo;
	{
		vk_renderPassBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		vk_renderPassBeginInfo.pNext = nullptr;
		vk_renderPassBeginInfo.renderPass = renderPass->GetHandle();
		vk_renderPassBeginInfo.framebuffer = framebuffer->GetHandle();
		vk_renderPassBeginInfo.renderArea = area;
		vk_renderPassBeginInfo.clearValueCount = clearValues.size();
		vk_renderPassBeginInfo.pClearValues = clearValues.data();
	};

	vkCmdBeginRenderPass(
		commandBuffer_->GetHandle(),
		&vk_renderPassBeginInfo,
		content);

	for(auto &command : commands)
	{
		command->Parse(commandBuffer_);
	}

	vkCmdEndRenderPass(commandBuffer_->GetHandle());
}
#pragma endregion
#pragma region Draw
void GreatVEngine::Vulkan::Commands::Draw::Parse(CommandBuffer* commandBuffer_)
{
	vkCmdDraw(
		commandBuffer_->GetHandle(),
		verticesCount, instancesCount,
		verticesBegin, instancesBegin);
}
#pragma endregion
#pragma region DrawIndices
void GreatVEngine::Vulkan::Commands::DrawIndexed::Parse(CommandBuffer* commandBuffer_)
{
	vkCmdDrawIndexed(
		commandBuffer_->GetHandle(),
		indicesCount, instancesCount,
		indicesBegin, 0, instancesBegin);
}
#pragma endregion
#pragma endregion


#pragma region
#pragma endregion
