#include "vulkan_renderpass.h"

#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/logging.h"
#include "vulkan/vulkan_core.h"

b8 vulkan_renderpass_create(vulkan_context* context,
                            vulkan_renderpass* out_renderpass,
                            rect_2d render_area, colour clear_colour,
                            f32 clear_depth, u32 clear_stencil) {
    out_renderpass->render_area = render_area;
    out_renderpass->clear_colour = clear_colour;
    out_renderpass->clear_depth = clear_depth;
    out_renderpass->clear_stencil = clear_stencil;
    
    // First create the subpassess
    // Main subpass
    VkSubpassDescription main_subpass = {0};
    // We are saying that we are going to bind this subpass to the graphics pipeline
    main_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    // We are also going to need all the attachments for this subpass
    // TODO: Make this configurable
    u32 attachment_count = 2;
    VkAttachmentDescription attachment_descriptions[attachment_count];
    // Colour attachment
    VkAttachmentDescription colour_attachment = {0};
    colour_attachment.format = context->swapchain.format.format; // TODO: Make this configurable
    colour_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    // We are going to clear the colour attachment at the start of the renderpass
    // There are other options for this, but for now we are just going to clear it
    // We could do VK_ATTACHMENT_LOAD_OP_DONT_CARE which would mean that we don't care about the previous contents
    // We could also do VK_ATTACHMENT_LOAD_OP_LOAD which would mean that we want to preserve the previous contents
    colour_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    // We are going to store the contents of the colour attachment at the end of the renderpass
    // There are other options for this, but for now we are just going to store it
    colour_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    // We are nto using stencil so we are going to set the load and store to DONT_CARE
    colour_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colour_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // We are going to set the initial layout to UNDEFINED as we dont expect any previous contents
    colour_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    // We are going to set the final layout to PRESENT_SRC_KHR as we are going to present the image to the screen
    colour_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colour_attachment.flags = 0;

    attachment_descriptions[0] = colour_attachment;

    // We need a reference to the attachment in the subpass as the actual description is stored in the renderpass
    VkAttachmentReference colour_attachment_ref = {0};
    colour_attachment_ref.attachment = 0; // This is the index of the attachment in the attachment descriptions
    // We are going to set the layout to colour attachment optimal as we are going to use it as a colour attachment
    colour_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    main_subpass.colorAttachmentCount = 1;
    main_subpass.pColorAttachments = &colour_attachment_ref;

    // Depth attachment
    VkAttachmentDescription depth_attachment = {0};
    depth_attachment.format = context->device.depth_format; // TODO: Make this configurable
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    // We are going to clear the depth attachment at the start of the renderpass
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachment_descriptions[1] = depth_attachment;

    VkAttachmentReference depth_attachment_ref = {0};
    depth_attachment_ref.attachment = 1; // This is the index of the attachment in the attachment descriptions
    // We are going to set the layout to depth attachment optimal as we are going to use it as a depth attachment
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    main_subpass.pDepthStencilAttachment = &depth_attachment_ref;

    // TODO: There are other attachments such as input attachments, resolve attachments, and preserve attachments
    // We are not going to use these for now

    // Input attachments are used for input from a shader
    main_subpass.inputAttachmentCount = 0;
    main_subpass.pInputAttachments = NULL;

    // Resolve attachments are used for multisampling colour attachments
    main_subpass.pResolveAttachments = NULL;

    // Preserve attachments are used to preserve the contents for the next subpass but are not used in the current subpass
    main_subpass.preserveAttachmentCount = 0;
    main_subpass.pPreserveAttachments = NULL;

    VkSubpassDependency subpass_dependency = {0};
    // Describes the source of this subpass but since this is the first subpass we are going to use VK_SUBPASS_EXTERNAL
    subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    // Describes the destination of this subpass but since this is the first and only subpass we are going to use 0
    subpass_dependency.dstSubpass = 0;
    // We are going to use the colour attachment output stage as the source stage and destination stage
    subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.srcAccessMask = 0;
    subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    // We want to be able to read and write to the colour attachment
    subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpass_dependency.dependencyFlags = 0;
    
    VkRenderPassCreateInfo renderpass_create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    renderpass_create_info.attachmentCount = attachment_count;
    renderpass_create_info.pAttachments = attachment_descriptions;
    renderpass_create_info.subpassCount = 1;
    renderpass_create_info.pSubpasses = &main_subpass;
    renderpass_create_info.dependencyCount = 1;
    renderpass_create_info.pDependencies = &subpass_dependency;
    renderpass_create_info.pNext = NULL;
    renderpass_create_info.flags = 0;

    VK_CHECK_RESULT(vkCreateRenderPass(
        context->device.logical_device, &renderpass_create_info,
        context->allocator, &out_renderpass->handle));
    FR_CORE_INFO("Renderpass created successfully");
    return TRUE;
}

void vulkan_renderpass_destroy(vulkan_context* context, vulkan_renderpass* renderpass) {
    if (renderpass && renderpass->handle != VK_NULL_HANDLE){
        vkDestroyRenderPass(context->device.logical_device, renderpass->handle,
                            context->allocator);
        renderpass->handle = VK_NULL_HANDLE;
        FR_CORE_INFO("Renderpass destroyed successfully");
        return;
    }
    FR_CORE_WARN("Attempting to destyroy a Renderpass handle that is NULL or not allocated");
}

void vulkan_renderpass_begin(vulkan_renderpass* renderpass,
                             vulkan_frame_buffer* framebuffer,
                             vulkan_command_buffer* command_buffer) {
    VkRenderPassBeginInfo renderpass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    renderpass_begin_info.renderPass = renderpass->handle;
    renderpass_begin_info.framebuffer = framebuffer->handle;
    renderpass_begin_info.renderArea.offset.x = renderpass->render_area.x;
    renderpass_begin_info.renderArea.offset.y = renderpass->render_area.y;
    renderpass_begin_info.renderArea.extent.width = renderpass->render_area.width;
    renderpass_begin_info.renderArea.extent.height = renderpass->render_area.height;

    // We are going to clear the colour and depth attachments hence 2 clear values
    VkClearValue clear_values[2];
    // Just to be safe and make sure we are not writing garbage for the depth
    // and stencil if we are not clearing them
    fr_memory_zero(clear_values, sizeof(VkClearValue) * 2); 
    clear_values[0].color.float32[0] = renderpass->clear_colour.r;
    clear_values[0].color.float32[1] = renderpass->clear_colour.g;
    clear_values[0].color.float32[2] = renderpass->clear_colour.b;
    clear_values[0].color.float32[3] = renderpass->clear_colour.a;
    clear_values[1].depthStencil.depth = renderpass->clear_depth;
    clear_values[1].depthStencil.stencil = renderpass->clear_stencil;

    renderpass_begin_info.clearValueCount = 2;
    renderpass_begin_info.pClearValues = clear_values;

    vkCmdBeginRenderPass(command_buffer->handle, &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    command_buffer->state = COMMAND_BUFFER_STATE_IN_RENDER_PASS;
}

void vulkan_renderpass_end(vulkan_renderpass *renderpass,
                           vulkan_command_buffer *command_buffer) {
    vkCmdEndRenderPass(command_buffer->handle);
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING;
}